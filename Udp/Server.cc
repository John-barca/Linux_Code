//先实现UDP版本的服务器
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){
    //1.先创建一个socket
    //AF_INET是一个宏，表示使用ipv4协议
    //SOCK_DGRAM表示使用UDP协议
    int sock = socket(AF_INET,SOCK_DGRAM,0);
    if(sock < 0){
        perror("socket");
        return 1;
    }
    //2.把当前socket绑定ip + 端口号
    sockaddr_in addr;
    addr.sin_family = AF_INET;//family设置协议家族
    //ip地址是一个整数，我们需要把它转成网络字节序
    //不过inet_addr函数自动帮我们转了
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    //端口号必须先转成网络字节序
    addr.sin_port = htons(9090);

    int ret = bind(sock, (sockaddr*)&addr, sizeof(addr));
    if(ret < 0){
        perror("bind");
        return 1;
    }
    printf("server start ok!\n");
    //3.处理服务器收到的请求
    while(true){
        //a)读取客户端请求
        //面向数据报的函数接口
        sockaddr_in peer;//对端
        socklen_t len = sizeof(peer);//输入输出参数
        char buf[1024] = {0};
        ssize_t n = recvfrom(sock, buf, sizeof(buf) - 1, 0, (sockaddr*)&peer, &len);
        if(n < 0){
            perror("recvfrom");
            continue;//考虑到容错，不要因为一次的失利就结束程序
        }
        buf[n] = '\0';
        //b)根据请求内容计算响应
        //[略]此处为回显服务器
        //c)把响应写回客户端
        n = sendto(sock, buf, strlen(buf), 0, (sockaddr*)&peer, len);
        if(n < 0){
            perror("sendto");
            continue;
        }
        printf("[%s:%d]buf: %s\n", inet_ntoa(peer.sin_addr),ntohs(peer.sin_port),buf);
    }
    close(sock);
    return 0;
}