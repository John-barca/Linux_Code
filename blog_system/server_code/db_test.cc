#include "db.hpp"

//使用测试程序，检测刚才写的MySQL程序是否正确
//单元测试
void TestBlogSystem(){
    Json::StyledWriter writer;
    MYSQL* mysql = blog_system::MySQLInit();
    blog_system::BlogTable blog_table(mysql);
    Json::Value blog;
    
    //测试插入
    blog["title"] = "我的第一篇博客";
    blog["content"] = "我要拿三十";
    blog["tag_id"] = 1;
    blog["create_time"] = "2019/07/28";
    bool ret = blog_table.Insert(blog);
    printf("insert: %d\n", ret);

    //测试查找多条
//    Json::Value blogs;
//    bool r = blog_table.SelectAll(&blogs);
//    printf("select all %d\n", ret);
//    printf("blogs: %s\n", writer.write(blogs).c_str());
//
//    ret = blog_table.Delete(1);
//    printf("Delete: %d", ret);
//
//
//    blog_system::MySQLRelease(mysql);
//}
//
//void TestTagTable(){
//    MYSQL* mysql = blog_system::MySQLInit();
//    blog_system::TagTable tagtable(mysql);
//    Json::Value tag;
//    tag["tag_name"] = "C语言";
//    bool ret = tagtable.Insert(tag);
//    printf("insert: %d\n", ret);
//
//    
//
//    blog_system::MySQLRelease(mysql);
}

int main(){
    TestBlogSystem();
    return 0;
}