#include "httplib.h"
#include "db.hpp"
#include <signal.h>

MYSQL* mysql = NULL;

int main(){
    using namespace httplib;
    using namespace blog_system;
    //1.先和数据库建立好连接
    mysql = blog_system::MySQLInit();
    signal(SIGINT, [](int){
        blog_system::MySQLRelease(mysql);
        exit(0);
    });
    //2.创建相关数据库处理对象
    BlogTable blog_table(mysql);
    TagTable tag_table(mysql);
    //3.创建服务器，并设置 “路由”(http服务器中的路由，和IP协议的路由不一样)，此处的路由指的是把方法+path => 哪个处理函数相关联声明清楚
    Server server;
    //新增博客
    server.Post("/blog", [&blog_table](const Request& req, Response& resp){
        printf("新增博客!\n");
        //1.获取到请求中的body 并解析成json
        //字符串 - > json 对象 reader
        //json - > 字符串 writer
        Json::Reader reader;
        Json::FastWriter writer;
        Json::Value req_json;
        Json::Value resp_json;
        bool ret = reader.parse(req.body, req_json);
        if(!ret){
            // 解析出错，给用户提示
            printf("解析请求失败! %s\n", req.body.c_str());
            // 构造一个响应对象， 告诉客户端出错了
            resp_json["ok"] = false;
            resp_json["reason"] = "input data format error!";
            resp.status = 400;
            resp.set_content(writer.write(resp_json), "application/json");
            return;
        }
        //2.对参数进行校验
        if(req_json["title"].empty() || req_json["content"].empty() || req_json["tag_id"].empty() || req_json["create_time"].empty()){
            printf("请求数据格式有误! %s\n", req.body.c_str());
            // 构造一个响应对象， 告诉客户端出错了
            resp_json["ok"] = false;
            resp_json["reason"] = "input data format error!";
            resp.status = 400;
            resp.set_content(writer.write(resp_json), "application/json");
            return;
        }
        //3.真正的调用MySQL接口来操作
        ret = blog_table.Insert(req_json);
        if(!ret){
            printf("博客插入失败!");
            resp_json["ok"] = false;
            resp_json["reason"] = "blog insert failed!";
            resp.status = 500;
            resp.set_content(writer.write(resp_json), "application/json");
        }
        //4.构造一个正确的响应给客户端即可
        printf("博客插入成功!\n");
        resp_json["ok"] = true;
        resp.set_content(writer.write(resp_json), "application/json");
    });
    //查看所有博客列表
    server.Get("/blog", [&blog_table](const Request& req, Response& resp){
        printf("查看所有博客！\n");
        Json::FastWriter writer;
        //1. 尝试获取tag_id,如果tag_id这个参数不存在，返回空字符串
        const std::string& tag_id = req.get_param_value("tag_id");
        //就不需要解析请求了，也就不需要合法性判定
        //2.调用数据库操作来获取所有博客结果
        Json::Value resp_json;
        bool ret = blog_table.SelectAll(&resp_json, tag_id);
        if(!ret){
            resp_json["ok"] = false;
            resp_json["reason"] = "select all failed";
            resp.status = 500;
            resp.set_content(writer.write(resp_json), "application/json");
            return;
        }
        //3.构造响应结果
        resp_json["ok"] = true;
        resp.set_content(writer.write(resp_json), "application/json");
        return;
    });
    //查看某个博客
    server.Get(R"(/blog/(\d+))", [&blog_table](const Request& req, Response& resp){
        Json::FastWriter writer;
        //1.解析获取到blog_id
        //printf("%s,%s\n", req.matches[0].str().c_str(), req.matches[1].str().c_str());
        //[0] :表示具体的路径
        int32_t blog_id = std::stoi(req.matches[1].str());
        printf("查看id为%d的博客\n", blog_id);
        //2.直接调用数据库操作
        Json::Value resp_json;
        bool ret = blog_table.SelectOne(blog_id, &resp_json);
        if(!ret){
            resp_json["ok"] = false;
            resp_json["reason"] = "查看指定博客失败: " + std::to_string(blog_id);
            resp.status = 404;
            resp.set_content(writer.write(resp_json), "application/json");
        }
        //3.包装一个执行正确的响应
        resp_json["ok"] = true;
        resp.set_content(writer.write(resp_json), "application/json");
        return;
    });
    //修改某个博客
    server.Put(R"(/blog/(\d+))", [&blog_table](const Request& req, Response& resp){
        //1.先获取到博客id
        int32_t blog_id = std::stoi(req.matches[1].str());
        printf("修改id为%d的博客！\n", blog_id);
        //2.获取到请求并解析结果
        Json::Reader reader;
        Json::FastWriter writer;
        Json::Value req_json;
        Json::Value resp_json;
        bool ret = reader.parse(req.body, req_json);
        if(!ret){
            resp_json["ok"] = false;
            resp_json["reason"] = "update blog parse request failed";
            resp.status = 400;
            resp.set_content(writer.write(resp_json), "application/json");
            return;
        }
        //3.校验参数是否符合预期
        if(req_json["title"].empty() || req_json["content"].empty() || req_json["tag_id"].empty()){
            resp_json["ok"] = false;
            resp_json["reason"] = "update blog data format failed";
            resp.status = 400;
            resp.set_content(writer.write(resp_json), "application/json");
            return;
        } 
        //4.调用数据库操作来完成更新博客操作
        req_json["blog_id"] = blog_id;
        ret = blog_table.Update(req_json);
        if(!ret){
            resp_json["ok"] = false;
            resp_json["reason"] = "update blog database failed";
            resp.status = 400;
            resp.set_content(writer.write(resp_json), "application/json");
            return;
        }
        //5.构造一个正确的返回结果
        resp_json["ok"] = true;
        resp.set_content(writer.write(resp_json), "application/json");
        return;
    });
    //删除某个博客
    server.Delete(R"(/blog/(\d+))", [&blog_table](const Request& req, Response& resp){
        //1. 获取到blog_id
        int32_t blog_id = std::stoi(req.matches[1].str());
        printf("删除id为 %d 的博客！\n", blog_id);
        //2.调用数据库操作
        bool ret = blog_table.Delete(blog_id);
        Json::Value resp_json;
        Json::FastWriter writer;
        if(!ret){
            printf("执行删除操作失败!\n");
            resp_json["ok"] = false;
            resp_json["reason"] = "update blog database failed";
            resp.status = 400;
            resp.set_content(writer.write(resp_json), "application/json");
            return;
        }
        resp.set_content(writer.write(resp_json), "application/json");
        return;
    });

    //新增标签
    server.Post("/tag", [&tag_table](const Request& req, Response& resp){
        Json::FastWriter writer;
        Json::Reader reader;
        Json::Value req_json;
        Json::Value resp_json;
        //1.解析请求
        bool ret = reader.parse(req.body, req_json);
        if(!ret){
            printf("新增标签失败!\n");
            resp_json["ok"] = false;
            resp_json["reason"] = "insert tag failed";
            resp.status = 400;
            resp.set_content(writer.write(resp_json), "application/json");
            return;
 
        }
        //2.对请求进行校验
        if(req_json["tag_name"].empty()){
            printf("新增标签失败!\n");
            resp_json["ok"] = false;
            resp_json["reason"] = "insert tag failed";
            resp.status = 400;
            resp.set_content(writer.write(resp_json), "application/json");
            return;
        }
        //3.调用数据库操作完成插入
        ret = tag_table.Insert(req_json);
        if(!ret){
            printf("新增标签失败!\n");
            resp_json["ok"] = false;
            resp_json["reason"] = "update blog database failed";
            resp.status = 400;
            resp.set_content(writer.write(resp_json), "application/json");
            return;
        }
        //4.返回正确的结果
        printf("新增标签成功!\n");
        resp_json["ok"] = true;
        resp.set_content(writer.write(resp_json), "application/json");
        return;
    });
    //删除标签
    server.Delete(R"(/tag/(\d+))", [&tag_table](const Request &req, Response &resp) {
        //1.解析出tag_id
        Json::FastWriter writer;
        Json::Value resp_json;
        int32_t tag_id = std::stoi(req.matches[1].str());
        printf("删除tag_id为%d的标签!\n", tag_id);
        //2.执行数据库操作哟
        bool ret = tag_table.Delete(tag_id);
        if(!ret){
            printf("执行删除操作失败!\n");
            resp_json["ok"] = false;
            resp_json["reason"] = "update blog database failed";
            resp.status = 500;
            resp.set_content(writer.write(resp_json), "application/json");
            return;
        }
        printf("删除标签成功!\n");
        resp_json["ok"] = true;
        resp.set_content(writer.write(resp_json), "application/json");
        return;
    });
    //查看标签
    server.Get("/tag", [&tag_table](const Request &req, Response &resp) {
        //1.不需要解析参数，直接执行数据库操作
        Json::FastWriter writer;
        Json::Value resp_json;
        bool ret = tag_table.SelectAll(&resp_json);
        if(!ret){
            printf("获取标签失败!\n");
            resp_json["ok"] = false;
            resp_json["reason"] = "select tag failed";
            resp.status = 500;
            resp.set_content(writer.write(resp_json), "application/json");
            return;
        }
        //2.构造正确的结果
        resp.set_content(writer.write(resp_json), "application/json");
        return;
    });

    server.set_base_dir("./wwwroot");
    server.listen("0.0.0.0", 9093);
    return 0;
}