/*
 * @Author: hanhaodong 
 * @Date: 2021-07-30 14:13:25 
 * @Last Modified by: hanhaodong
 * @Last Modified time: 2021-07-30 15:12:50
 */
#include "mySocket.h"

void mySocketCli(int *socketFd, int type, char *ip, int port, SA *myAddr) {
  ASSERT(type == MY_TCP || type == MY_UDP, "invalid arg\n");
  if (type == MY_TCP) {
    ASSERT((*socketFd = socket(AF_INET, SOCK_STREAM, 0)) != -1, "TCP SOCKET INIT FAIL!\n");
  } else if (type == MY_UDP) {
    ASSERT((*socketFd = socket(AF_INET, SOCK_DGRAM, 0)) != -1, "UDP SOCKET INIT FAIL!\n");
  }
  memset(myAddr, 0, sizeof(SA));
  (*myAddr).sin_family = AF_INET;
  (*myAddr).sin_port = htons(port);
  (*myAddr).sin_addr.s_addr = inet_addr(ip);
  return;
}

void mySocketServer(int* socketFd, int type, char* ip, int port) {
  ASSERT(type == MY_UDP || type == MY_UDP, "invalid arg\n");
  if (type == MY_TCP) {
    ASSERT((*socketFd = socket(AF_INET, SOCK_STREAM, 0)) != -1, "TCP SOCKET INIT FAIL!\n");
  } else if (type == MY_UDP) {
    ASSERT((*socketFd = socket(AF_INET, SOCK_DGRAM, 0)) != -1, "UDP SOCKET INIT FAIL!\n");
  }
  SA myAddr;
  memset(&myAddr, 0, sizeof(myAddr));
  myAddr.sin_family = AF_INET;

  printf("PORT:%d, IP:%s\n", port, ip);

  myAddr.sin_port = htons(port);
  myAddr.sin_addr.s_addr = inet_addr(ip);
  int len = sizeof(myAddr);

  ASSERT(bind(*socketFd, (struct sockaddr*)&myAddr, len) != -1, "bind fail\n");

  return;
}

void myListen(int socketFd, int listenNum) {
  ASSERT((listen(socketFd, listenNum)) != -1, "LISTEN FAIL!\n");
}

void myAccept(int *clientFd, int socketFd, pSA clientAddr, int* addrlen) {
  //ASSERT((*clientFd = accept(socketFd, clientAddr, addrlen)) != -1, "SERVER ACCEPT FAIL!\n");
  ASSERT((*clientFd = accept(socketFd, clientAddr, addrlen)) != -1, "SERVER ACCEPT FAIL\n");
}

void mySend(int *sendlen, int socketFd, int *msg, int len) {
  ASSERT((*sendlen = send(socketFd, msg, len, 0)) != -1, "SEND FAIL\n");
}

void myRecv(int *recvlen, int socketFd, int *msg, int len) {
  ASSERT((*recvlen = recv(socketFd, msg, len, 0)) != -1, "RECV FAIL!\n");
}

void myClose(int socketFd) {
  close(socketFd);
}

void myConnect(int socketFd, pSA serverAddr, int len) {
  connect(socketFd, serverAddr, len);
}