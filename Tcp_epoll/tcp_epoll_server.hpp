#pragma once
#include "tcp_socket.hpp"
#include <sys/epoll.h>
#include <vector>
#include <functional>

//Epoll这个类就对标 Selector 这个类
class Epoll{
public:
    Epoll(){
        epoll_fd_ = epoll_create(10);
    }

    ~Epoll(){
        close(epoll_fd_);
    }

    //只有新客户端连接过来了才调用,调用频率远低于select
    //也就意味着epoll拷贝数据就远没有select拷贝的那么频繁
    void Add(const TcpSocket& sock, bool is_et = false) {
        printf("[Epoll :: Add] %d\n", sock.GetFd());
        epoll_event event;
        event.events = EPOLLIN;
        if(is_et == true){
            event.events |= EPOLLET;
        }
        //此处epoll add的时候插入的是键值对
        //fd在键和值之中都出现了
        event.data.fd = sock.GetFd();   //event.data是一个联合体
        epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, sock.GetFd(), &event);
    }

    void Del(const TcpSocket& sock){
        printf("[Epoll :: Del] %d\n", sock.GetFd());
        epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, sock.GetFd(), NULL);
    }   

    void Wait(std::vector<TcpSocket>* output){
        output->clear();
        //等待文件描述符就绪
        epoll_event events[100];
        //最后一个参数表示阻塞等待
        //返回之后，就有若干个文件描述符就绪，保存在events数组之中
        //返回值结果就是在描述数组中有几个有效元素
        //epoll_wait返回的内容只是我们键值对的值
        //如果不加任何处理的话，我们用户不知道对应的文件描述符是谁
        //迫不得已，只能在插入的时候，把socket往值里也存一份
        int nfds = epoll_wait(epoll_fd_, events, 100, -1);
        if(nfds < 0){
            perror("epoll_wait");
            return;
        }

        //依次处理每个就绪的文件描述符
        for(int i = 0; i < nfds; ++i){
            int sock = events[i].data.fd;
            output->push_back(TcpSocket(sock));
        }
    } 
private:
    int epoll_fd_; //通过这个epoll_fd找到内核中的对象，从而进行文件描述符的管理
};

typedef std::function<void(const std::string&, std::string* )> Handler;

#define CHECK_RET(exp) if(!exp){\
    return false;\
}

class TcpEpollServer{
public:
    bool Start(const std::string& ip, uint16_t port, Handler handler){
        //1.先创建socket
        TcpSocket listen_sock;
        CHECK_RET(listen_sock.Socket());
        //2.绑定端口号
        CHECK_RET(listen_sock.Bind(ip, port));
        //3.监听socket
        CHECK_RET(listen_sock.Listen());
        //4.创建Epoll 对象，并把listen_sock 用Epoll管理起来
        Epoll epoll;
        epoll.Add(listen_sock);
        //5.进入主循环
        while(true){
            //6.使用Epoll::Wait等待文件描述符就绪
            std::vector<TcpSocket> output;
            epoll.Wait(&output);
            //7.循环处理每个就绪的文件描述符，也是分成两种情况
            for(auto sock : output){
                if(sock.GetFd() == listen_sock.GetFd()){
                    //a) listen_sock 就调用accept
                    TcpSocket client_sock;
                    sock.Accept(&client_sock);
                    epoll.Add(client_sock);
                } else {
                    //b) 非listen_sock，就调用Recv
                    std::string req;
                    //ET模式下必须采用非阻塞的方式进行读写
                    int n = sock.Recv(&req);
                    if(n < 0){
                        continue;
                    }
                    if(n == 0){
                        //对端关闭
                        printf("[client %d] disconnected\n", sock.GetFd());
                        sock.Close();
                        epoll.Del(sock);
                        continue;
                    }
                    printf("[client %d] %s\n", sock.GetFd(), req.c_str());
                    //正确读取的情况
                    std::string resp;
                    handler(req, &resp);
                    sock.Send(resp);
                }//END ELSE
            }//END FOR
       }//END WHILE(true)
   }//end function Start
};

//LT 水平触发：代码写起来比较简单（默认工作方式）
//ET 边缘触发：某些情况下效率更高，代码会更复杂
