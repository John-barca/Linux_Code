#include <stdio.h>
#include <stdlib.h>
#include <cstdarg>
#include <cstddef>
#include <string.h>
#include <assert.h>
#include <hiredis/hiredis.h>

void doTest() {
  // redis 默认监听 6379
  redisContext* c = redisConnect("127.0.0.1", 6379);
  if (c->err) {
    redisFree(c);
    printf("Connect to redisServer fail\n");
    return;
  }
  printf("Connect to redisServer Success\n");

  // 测试当前 redis 连接是否成功
  const char* command1 = "set stest1 value1";
  redisReply* r = (redisReply*)redisCommand(c, command1);

  if (NULL == r) {
    printf("Execut command1 failure\n");
    redisFree(c);
    return;
  }

  // 测试当前 hiredis 库， redis 命令执行是否正常
  if (!(r->type == REDIS_REPLY_STATUS && strcasecmp(r->str, "OK") == 0)) {
    printf("Failed to execute command[%s]\n", command1);
    freeReplyObject(r);
    redisFree(c);
    return;
  }

  freeReplyObject(r);
  printf("Succeed to execute command[%s]\n", command1);

  const char* command2 = "strlen stest1";
  r = (redisReply*)redisCommand(c, command2);
  if (r->type != REDIS_REPLY_INTEGER) {
    printf("Failed to execute command[%s]\n", command2);
    freeReplyObject(r);
    redisFree(c);
    return;
  }

  int length = r->integer;
  freeReplyObject(r);
  printf("The length of 'stest1' is %d, \n", length);
  printf("Succeed to execute command[%s]\n", command2);

  const char* command3 = "get stest1";
  r = (redisReply*)redisCommand(c, command3);
  if (r->type != REDIS_REPLY_STRING) {
    printf("Failed to execute command[%s]\n", command3);
    freeReplyObject(r);
    redisFree(c);
    return;
  }
  printf("The value of 'stest1' is %s\n", r->str);
  freeReplyObject(r);
  printf("Succeed to execute command[%s]\n", command3);

  const char* command4 = "get stest2";
  r = (redisReply*)redisCommand(c, command4);
  if (r->type != REDIS_REPLY_NIL) {
    printf("Failed to execute command[%s]\n", command4);
    freeReplyObject(r);
    redisFree(c);
    return;
  }

  freeReplyObject(r);
  printf("Succeed to execute command[%s]\n", command4);


  redisFree(c);
}

int main() {
  doTest();
  return 0;
}

