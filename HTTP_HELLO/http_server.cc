#include "tcp_thread.hpp"

void HttpProcess(const std::string& req, std::string* resp){
    //不管用户输入的是什么请求，都返回hello world
    (void)req;
    std::string first_line = "HTTP/1.1 302 Found\n";
    //std::string body = "<html><div>hello world</div></html>";
    //std::string header = "Content-Type: text/html\nContent-Length: " + std::to_string(body.size()) + "\n";
    std::string header = "Location: http://www.baidu.com\n"  "Content-Type: text/html\n" "Content-Length: 10\n";
    std::string body = "aaaaaaaaaa\n";
    *resp = first_line + header + "\n" + body;
}

int main(){
    TcpThreadServer server;
    server.Start("0.0.0.0", 9090, HttpProcess);


    return 0;
}