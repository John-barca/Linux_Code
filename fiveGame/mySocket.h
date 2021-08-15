/*
 * @Author: hanhaodong 
 * @Date: 2021-07-30 14:14:58 
 * @Last Modified by:   hanhaodong 
 * @Last Modified time: 2021-07-30 14:14:58 
 */
#ifndef __MY_SOCKET_H__
#define __MY_SOCKET_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IN
#define OUT
#define IN_OUT

#define MY_TCP 1
#define MY_UDP 2

typedef struct sockaddr* pSA;
typedef struct sockaddr_in SA;

#define ASSERT(flag,msg) ((flag)?NULL:(fprintf(stdout,(msg)),exit(EXIT_FAILURE)))//如果flag为假，则执行后面
void mySocketCli(int *socketFd, int type, char* ip, int port, SA *myAddr); // tcp/udp客户端不需要绑定，但得把存有server的addr发过去，所以的将addr传出
void myConnect(int socketFd, pSA serverAddr, int len); // tcp 客户端链接服务器
void mySocketServer(int *socketFd, int type, char *ip, int port); // tcp/udp服务端需要自己绑定addr
void myListen(int socketFd, int listenNum); // 监听
void myAccept(int *clientFd, int socketFd, pSA clientAddr, int* addrlen); // tcp服务器将接受客户端发来的信号，并将信息保存到client_addr结构体中，以后用 fd_client 链接服务器
void mySend(int* sendlen, int socketFd, int *msg, int len); // tcp, msg 是一个int数组
void myRecv(int* recvlen, int socketFd, int *msg, int len); // tcp
void mySendto(int *sendNum, int socketFd, int *msg, int len, pSA serverAddr, int addrlen); // udp 使用
void myRecvfrom(int *recvNum, int socketFd, int *msg, int len, pSA clientAddr, int *addrlen); // udp 使用
void myClose(int socketFd); // 关闭fd

#endif