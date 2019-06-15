//#pragma once
#include <unordered_map>
#include "tcp_process.hpp"

int main(){
    std::unordered_map<std::string, std::string> dict;
    dict.insert(std::make_pair("good", "好"));
    dict.insert(std::make_pair("baidu", "百度"));
    dict.insert(std::make_pair("Tencent", "腾讯"));
    dict.insert(std::make_pair("Byte", "字节"));
    TcpProcess_server server;
    server.Start("0.0.0.0", 9090, [&dict](const std::string& req, std::string* resp){
        auto it = dict.find(req);
        if(it == dict.end()){
            *resp = "未找到";
        }else{
            *resp = it->second;
        }
    });
    
    return 0;
}