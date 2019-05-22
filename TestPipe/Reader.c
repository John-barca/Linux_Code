//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <string.h>
//
//int main()
//{
//  int fds[2];
//  char buf[1024] = {0};
//
//  if(pipe(fds) == -1)
//  {
//    perror("make pipe");
//  }
//  //从标准输入读取数据
//  while(fgets(buf,sizeof(buf),stdin))
//  {
//    int len = strlen(buf);
//    //将数据写入管道
//    if(write(fds[1],buf,len)!=len)
//    {
//      perror("write to pipe");
//      break;
//    }
//    memset(buf,0x00,sizeof(buf));
//
//    //read from pipe
//    if((len = read(fds[0],buf,1024)==-1))
//    {
//      perror("read from pipe");
//      break;
//    }
//    //write to stdout
//    if(write(1,buf,len)!=len)
//    {
//      perror("write to stdout");
//      break;
//    }
//  }
//  return 0;
//}
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>
//#include <fcntl.h>
//
//int main()
//{
//  char* buf = "hello mylife";
//  int fd = open("test.txt",O_RDONLY|O_CREAT,0644);
//  int count = 3;
//  while(count--)
//  {
//    write(fd,buf,strlen(buf));
//  }
//  close(fd);
//  return 0;
//}
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string.h>

int main()
{
  int fds[2];
  if(pipe(fds)==-1)
  {
    perror("make pipe");
  }

  pid_t pid;
  pid = fork();
  if(pid == -1)
  {
    perror("fork");
  }
  if(pid == 0)
  {
    //child
    close(fds[0]);
    write(fds[1],"hello",5);
    close(fds[1]);
  }
  else{
    //father
    close(fds[1]);
    char buf[10] = {0};
    read(fds[0],buf,10);
    printf("buf = %s\n",buf);
  }
  return 0;
}
