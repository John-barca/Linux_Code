#include <jsoncpp/json/json.h>
#include "tcp_thread.hpp"

//使用Json的方式进行序列化和反序列化
//json::value 用来表示一个json数据
//json::reader 把字符串转换成 json::value
//json::writer 把json::value转换成字符串


int main(){
    TcpThreadServer server;
    server.Start("0.0.0.0", 9090, [](const std::string& req, std::string* resp){
        //1.先将req进行反序列化
        Json::Reader reader;
        Json::Value req_json;
        reader.parse(req, req_json);
        //2.根据反序列化的结果，进行计算
        int num1 = req_json["num1"].asInt();
        int num2 = req_json["num2"].asInt();
        std::string op = req_json["op"].asString();
        int result = 0;
        if(op == "+"){
            result = num1 + num2;
        }else if(op == "-"){
            result = num1 - num2;
        }
        //3.把结果序列化回一个字符串
        Json::Value resp_json;
        Json::FastWriter writer;
        resp_json["result"] = result;
        *resp = writer.write(resp_json);
    });

    return 0;
}