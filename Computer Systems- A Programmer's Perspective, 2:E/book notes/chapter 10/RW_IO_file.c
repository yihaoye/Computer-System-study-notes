//一次一个字节地从标准输入拷贝到标准输出
#include "csapp.h"

int main(void)
{
	char c;

	while(Read(STDIN_FILENO, &c, 1) != 0)
		Write(STDOUT_FILENO, &c, 1);
	//Unix shell创建的每个进程开始都有三个打开的文件：标准输入（描述符为0），标准输出（描述符为1）和标准错误（描述符为2）。
	//头文件<unistd.h>定义了常量STDIN_FILENO, STDOUT_FILENO和STDERR_FILENO来代替显示的描述符值
	exit(0);
}





//http://webcache.googleusercontent.com/search?q=cache:XJftPv58quMJ:blog.csdn.net/mybelief321/article/details/8989755+&cd=2&hl=en&ct=clnk&gl=nz
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

main()
{
	int fd, size;
	char s[]="www.csapp.com",buffer[80]; //声明两个数组s[],buffer[]
	fd=open("/home/temp", O_WRONLY|O_CREAT);
	write(fd,s,sizeof(s));
	close(fd);

	fd=open("/home/temp", O_RDONLY);
	size=read(fd,buffer,sizeof(buffer)); //将fd指向的temp文件中的内容读出，存放到数组buffer中
	close(fd);
	printf("%s", buffer);
}