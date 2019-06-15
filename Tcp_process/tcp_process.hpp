#pragma once
//多进程
#include <functional>
#include <signal.h>
#include "tcp_socket.hpp"

typedef std::function<void(const std::string&, std::string*)> Handler;

#define CHECK_RET(exp) if(!(exp)){\
    return false;\
}

class TcpProcess_server{
public:
    TcpProcess_server(){

    }
    ~TcpProcess_server(){

    }

    bool Start(const std::string& ip, uint16_t port, Handler handler){
         // 1. 创建一个socket
        CHECK_RET(listen_sock.Socket());
        //2.绑定端口号
        CHECK_RET(listen_sock.Bind(ip, port));
        //3.进行监听
        CHECK_RET(listen_sock.Listen());

        while(true){
            TcpSocket client_sock;
            std::string peer_ip;
            uint16_t peer_port;
            bool ret = listen_sock.Accept(&client_sock, &peer_ip, &peer_port);
            if(!ret){
                continue;
            }
            printf("[%s:%d] 有客户端连接\n", ip.c_str(), port);
            Connect_Process(client_sock, peer_ip, peer_port, handler);
        }
    }
private:
    TcpSocket listen_sock;

    void Connect_Process(TcpSocket& client_sock, const std::string& ip, uint16_t port, Handler handler){
        signal(SIGCHLD,SIG_IGN);
        int ret = fork();
        if(ret > 0){
            return;
        }
        while(true){
            //a 读取请求
            std::string req;
            int r = client_sock.Recv(&req);
            if(r < 0){
                continue;
            }
            if(r == 0){
                //对端关闭
                client_sock.Close();
                printf("[%s:%d]对端关闭了连接\n",ip.c_str(),port);
                break;
            }
            printf("[%s:%d]客户端发送了请求: %s\n", ip.c_str(), port, req.c_str());
            //根据请求计算响应
            std::string resp;
            handler(req, &resp);
            //把响应写回客户端
            client_sock.Send(resp);
        }
    }
};