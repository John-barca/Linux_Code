//多线程版本的服务器
#include <functional>
#include <pthread.h>
#include "tcp_socket.hpp"

typedef std::function<void(const std::string&, std::string*)> Handler;

struct Arg{
    TcpSocket new_sock;
    std::string ip;
    uint16_t port;
    Handler handler;
};


#define CHECK_RET(exp) if(!(exp)){\
    return false;\
}

class TcpThreadServer{
public:
    TcpThreadServer(){

    }

    ~TcpThreadServer(){

    }

    bool Start(const std::string& ip, uint16_t port, Handler handler){
        //1.创建套接字
        CHECK_RET(listen_sock.Socket());
        //2.绑定端口号
        CHECK_RET(listen_sock.Bind(ip, port));
        //3.进行监听
        CHECK_RET(listen_sock.Listen());
        //4.进入主循环
        while(true){
            //5.通过accept获取一个连接
            
           // std::string peer_ip;
           // uint16_t peer_port;
           // bool ret = client_sock.Accept(&client_sock, &peer_ip, &peer_port);
           // if(!ret){
           //     continue;
           // }
           // printf("[%s:%d] 有客户端连接\n", ip.c_str(), port);
           // pthread_t tid;
           // tid = pthread_create(&tid, NULL, ThreadEntry, NULL); 
           Arg* arg = new Arg();
           arg->handler = handler;
           bool ret = listen_sock.Accept(&arg->new_sock, &arg->ip, &arg->port);
           if(!ret){
               continue;
           }
           printf("[%s:%d] 有客户端连接\n", arg->ip.c_str(), port);
           pthread_t tid;
           pthread_create(&tid, NULL, ThreadEntry, arg);
           pthread_detach(tid);
        }
        return true;
    }

    static void* ThreadEntry(void* arg){
        Arg* p = (Arg*) arg;

        while(true){
            //进入循环进行读写
            std::string req;
            //读取请求
            int r = p->new_sock.Recv(&req);
            if(r < 0){
                continue;
            }
            if(r == 0){
                printf("[%s:%d] 对端关闭了连接\n", p->ip.c_str(), p->port);
                break;
            }
            std::string resp;
            //根据请求计算响应
            p->handler(req, &resp);
            p->new_sock.Send(resp);
        }

        p->new_sock.Close();
        delete p;
        return NULL; 
    }

private:
    TcpSocket listen_sock;
};