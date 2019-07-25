/////////////////////////////////////////////////
//通过这个程序使用MySQL API 实现数据库的插入功能////
/////////////////////////////////////////////////

#include <cstdio>
#include <cstdlib>
//编译器默认从 /usr/include 目录中查找头文件
//mysql.h在mysql目录中
#include <mysql/mysql.h>

int main(){
    //1.创建一个数据库连接句柄
    MYSQL* connect_fd = mysql_init(NULL);
    //2.和数据库建立连接(和TCP区分开，这是在应用层建立连接)
    //  连接过程需要指定一些必要的信息
    //  a) 连接句柄
    //  b) 服务器的ip地址
    //  c) 用户名
    //  d) 密码
    //  e) 数据库名(blog_system)
    //  f) 服务器的端口号
    //  g) unix_sock: NULL
    //  h) client_flag: 0
    if(mysql_real_connect(connect_fd, "127.0.0.1", "root", "HHD123456789", "blog_system", 3306, NULL, 0) == NULL){
        printf("连接失败!\n", mysql_error(connect_fd));
        mysql_close(connect_fd);
        return 1;
    }
    printf("连接成功!\n");
    //3. 设置编码方式
    //   mysql server 部分最初安装的时候已经设置成了 utf8
    //   同时也得在客户端这边也设成utf8
    mysql_set_character_set(connect_fd, "utf8");
    //4. 拼接 SQL 语句
    char sql[1024 * 4] = {0};
    char title[] = "立一个flag";
    char content[] = "我要拿30w年薪";
    char tag_id = 1;
    char date[] = "2019/07/25";
    sprintf(sql, "insert into blog_table values(null, '%s', '%s', %d, '%s')", title, content, tag_id, date);
    printf("sql: %s\n", sql);

    //5. 让 数据库 服务器执行SQL
    int ret = mysql_query(connect_fd, sql);
    if(ret < 0){
        printf("执行SQL失败！ %s\n", mysql_error(connect_fd));
        mysql_close(connect_fd);
        return 1;
    }
    printf("插入成功!\n");

    //断开连接
    mysql_close(connect_fd);
    return 0;
}