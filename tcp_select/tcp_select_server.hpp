#pragma once
#include <vector>
//1.需要尽量高效的插入和删除
//2.需要方便得找到最大的元素
#include <set>
#include <functional>
#include <sys/select.h>
#include "tcp_socket.hpp"

//lhs 左操作数 rhs 右操作数
bool operator<(const TcpSocket& lhs, const TcpSocket& rhs){
    return lhs.GetFd() < rhs.GetFd();
}

//select这个函数本身用起来不方便，封装成一个类
//提供以下几个操作
//相当于文件描述符管理器,这个管理器中包含的socket就是要使用select来管理的socket
//1.新增一个文件描述符
//2.删除一个文件描述符
//3.等待操作 等待管理的文件描述符就绪（只考虑读就绪的情况）
class Selector{
public:

    void Add(const TcpSocket& sock){
        printf("[Selector::Add]%d\n",sock.GetFd());
        socket_set_.insert(sock);
    }

    void Del(const TcpSocket& sock){
        printf("[Selector::Del]%d\n",sock.GetFd());
        socket_set_.erase(sock);
    }

    //wait反悔的时候需要告诉调用者哪些文件描述符就绪了
    void Wait(std::vector<TcpSocket>* output){
        output->clear();
        //调用wait 就相当于调用select进行等待
        //先获取到最大的文件描述符
        if(socket_set_.empty()){
            printf("[Selector::Wait],Socket_set_ is empty");
            return;
        }
        int max_fd = socket_set_.rbegin()->GetFd();
        fd_set readfds;
        FD_ZERO(&readfds);
        //把set中的每个需要关注的文件描述符都设置到readfds之中
        for(const auto& sock : socket_set_){
            FD_SET(sock.GetFd(), &readfds);
        }
        //select默认是一个阻塞等待，有文件描述符返回的时候，才会进行返回
        //当这个函数返回之后，就应该根据文件描述符的返回情况
        //构造一个输出参数，告诉调用者哪些socket就绪了
        int nfds = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        if(nfds < 0){
            perror("select");
            return;
        }
        //已经返回了，返回结果同样也是保存在readfds之中
        for(int fd = 0; fd < max_fd + 1; ++fd){
            if(FD_ISSET(fd, &readfds)){
                //如果是，说明当前fd就是读就绪的
                //就把这个结果放到输出结果中
                output->push_back(TcpSocket(fd));
            }
        }
    }

private:
    //得用一个数据结构把需要的socket存起来
    //要求TcpSocket类能支持比较操作 (operator<)
    std::set<TcpSocket> socket_set_;
};

//实现一个select版本的TCP server

typedef std::function<void(const std::string&, std::string*)> Handler;

#define CHECK_RET(exp)if(!exp){\
    return false;\
}

class TcpSelectServer{
public:
    //一个服务器程序在处理请求的时候典型流程就是三个步骤
    //1.读取请求并解析
    //2.根据请求计算响应
    //3.把响应写回到客户端
    bool Start(const std::string& ip, uint16_t port, Handler handler){
        //1.创建socket
        TcpSocket listen_sock;
        CHECK_RET(listen_sock.Socket());
        //2.绑定端口号
        CHECK_RET(listen_sock.Bind(ip, port));
        //3.监听文件描述符
        CHECK_RET(listen_sock.Listen());
        //4.先创建一个Selector对象，让这个对象先把listen_sock管理起来
        //后续我们进行等待，都是靠selector对象完后曾
        Selector selector;
        selector.Add(listen_sock);
        //5.进入主循环
        while(true){
            //6.不再是直接调用accept，而是使用selector进行等待
            std::vector<TcpSocket> output;
            selector.Wait(&output);
            //7.遍历返回结果，依次处理每个就绪的socket
            for(auto tcp_socket : output){
                //8.分成两种情况讨论
                if(tcp_socket.GetFd() == listen_sock.GetFd()){
                    // a) 如果就绪socket是listen_sock要进行的操作是调用accep
                    TcpSocket client_sock;
                    std::string ip;
                    uint16_t port;
                    tcp_socket.Accept(&client_sock, &ip, &port);
                    selector.Add(client_sock);
                    printf("[client %s:%d] connected!\n", ip.c_str(), port);
                } else {
                    // b)如果就绪的socket不是Listen_sock 要进行的操作是调用recv
                    std::string req;
                    int n = tcp_socket.Recv(&req);
                    if(n < 0){
                        continue;
                    }
                    if(n == 0){
                        //对端关闭,也要把这个socket从selector中删掉
                        tcp_socket.Close();
                        selector.Del(tcp_socket);
                        continue;
                    }
                    printf("[client] %s\n", req.c_str());
                    std::string resp;
                    //根据请求计算响应
                    handler(req, &resp);
                    tcp_socket.Send(resp);
                }

            }
        }
    }
private:
};