//通用的TCP服务器
//#pragma once
#include "tcp_socket.hpp"
#include <functional>

#define CHECK_RET(exp) if(!(exp)){\
    return false;\
}

typedef std::function<void(const std::string&, std::string*)> Handler;

class TcpServer{
public:
    TcpServer(){
        
    }

    bool Start(const std::string& ip, uint16_t port,Handler handler){
        // 1. 创建一个socket
        CHECK_RET(listen_sock.Socket());
        //2.绑定端口号
        CHECK_RET(listen_sock.Bind(ip, port));
        //3.进行监听
        CHECK_RET(listen_sock.Listen());
        printf("server start ok\n");
        //4. 进入主循环
        while(true){
            //5.通过 accept 获取到一个链接
            TcpSocket client_socket;
            std::string ip;
            uint16_t port;
            //核心问题在于第一次 accept 之后就进入了一个循环
            //在这个操作过程中，循环一直没有结束，accept 没有被重复调用到
            //后续连接过来的客户端都在内核的连接队列中排队呢，一直得不到处理
            //应该想办法，让我们的程序能够更快速的调用到accept
            //多进程/ 多线程
            bool ret = listen_sock.Accept(&client_socket, &ip, &port);
            if(!ret){
                continue;
            }
            printf("[%s:%d] 有客户端连接\n", ip.c_str(), port);
            //6.和客户端进行具体的沟通,一次连接和客户端进行多次交互
            while(true){
                // a 读取请求
                std::string req;
                int r = client_socket.Recv(&req);
                if(r < 0){
                    continue;
                }
                if(r == 0){
                    //对端关闭了 socket
                    client_socket.Close();
                    printf("[%s:%d]对端关闭了连接\n", ip.c_str(), port);
                    break;
                }
                printf("[%s:%d] 客户端发送了: %s\n", ip.c_str(), port, req.c_str());
                // 根据请求计算响应
                std::string resp;
                handler(req, &resp);
                //c 把响应写回客户端
                client_socket.Send(resp);
            }
        }
        return true;
    }
private:
    TcpSocket listen_sock;
};
