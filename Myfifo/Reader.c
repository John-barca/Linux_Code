#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main()
{
  //对命名管道的操作和对文件的操作一模一样
  int fd = open("./myfifo",O_RDONLY);
  if(fd < 0)
  {
    perror("open");
    return 1;
  }

  while(1)
  {
    char buf[1024] = {0};
    ssize_t n = read(fd,buf,sizeof(buf)-1);
    if(n < 0)
    {
      perror("read");
      return 1;
    }
    buf[n] = '\0';
    printf("[reader]%s\n",buf);
  }

  close(fd);
  return 0;
}
