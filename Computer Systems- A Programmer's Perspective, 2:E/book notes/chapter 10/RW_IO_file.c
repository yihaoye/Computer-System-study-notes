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
	//当读到EOF信号时，说明已读完字节
	//理解read时可能遇到不足值的问题？？？
	close(fd);
	printf("%s", buffer);
}



/*
因为在写普通文件时，写操作从文件的当前指针位置开始。
下面是lseek函数较特别的使用（和write，read等函数同级）
☆ 欲将读写位置移到文件开头时：lseek(int fd,0,SEEK_SET)
☆ 欲将读写位置移到文件尾时：lseek(int fd,0,SEEK_END)
☆ 想要取得目前文件位置时：lseek(ind fd,0,SEEK_CUR)
*/

//可以简单的记住：ssize_t是有符号整形，size_t是无符号整形。



