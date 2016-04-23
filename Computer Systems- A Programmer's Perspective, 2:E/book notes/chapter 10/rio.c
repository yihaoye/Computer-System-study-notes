/* RIO (Robust I/O) */
/* I/O包 RIO包（健壮的I/O），会自动为你处理之前提及的不足值的问题，不足值问题会对创建可靠的web服务器等网络应用产生影响 */

/*
RIO提供了两类不同的函数：
1.无缓冲的输入输出函数：函数直接在存储器和文件之间传送数据。多应用于网络应用。
2.带缓冲的输入输出函数：有应用级缓冲，且线程安全，在同一描述符中可被交错调用。
*/


//RIO无缓冲的输入输出函数
#include "csapp.h"

ssize_t rio_readn(int fd, void *usrbuf, size_t n); //从描述符fd的当前文件位置最多传送n个字节到存储器位置usrbuf。
ssize_t rio_writen(int fd, void *usrbuf, size_t n);
//返回：若成功则为传送的字节数，若EOF则为0（只对rio_readn而言），若出错则为－1



//RIO带缓冲的输入输出函数
//计算文本文件中文本行的数量（文本行就是一个由换行符结尾的ASCII码字符序列）
#include "csapp.h"

void rio_readinitb(rio_t *rp, int fd); //每打开一个描述符都会调用一次rio_readinitb函数。它将描述符fd和地址rp处的一个类型为rio_t的读缓冲区联系起来。

ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen); 
//rio_readlineb（）从一个内部读缓冲区（地址rp处）靠背一个文本行，当缓冲区变空时，会自动地调用read重新填满缓冲区，比直接调用read函数减少陷入内核的次数，
//该函数一次最多读maxlen-1字节，余下一个字符留给结尾空字符。超过maxlen-1字节的文本行被截断。
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n); 
//rio_readnb（）可用于既包含文本行又包含二进制数据的文件处理

//返回：若成功则为读的字节数，若EOF则为0，若出错则为－1



//！！！注意：带缓冲的输入函数不要与无缓冲的输入函数对同一描述符交叉使用。





//如何使用RIO函数来一次一行从标准输入拷贝一个文本文件到标准输出
#include "csapp.h"

int main(int argc, char **argv)
{
	int n;
	rio_t rio;
	char buf[MAXLINE];

	rio_readinitb(&rio, STDIN_FILENO);
	while((n = rio_readlineb(&rio, buf, MAXLINE)) != 0)
		rio_writen(STDOUT_FILENO, buf, n);
}



//读缓冲区rio_t的格式 及 初始化它的rio_readinitb函数
#define RIO_BUFSIZE 8192
typedef struct{
	int rio_fd;						/* Descriptor for this internal buf */
	int rio_cnt;					/* Unread bytes in internal buf */
	char *rio_bufptr;				/* Next unread byte in internal buf */
	char rio_buf[RIO_BUFSIZE];		/* Internal buffer */
}rio_t


void rio_readinitb(rio_t *rp, int fd)
{
	rp->rio_fd = fd;
	rp->rio_cnt = 0;
	rp->rio_bufptr = rp->rio_buf;
}


//RIO读程序核心是rio_read函数，该函数是Unix read函数的带缓冲版本。每次读缓冲区为空时，调用read再填满它，
//如果read收到不足值且缓冲区只填充了一部分，rio_read就从读缓冲区拷贝min(n, rp->rio_cnt)个字节数到用户缓冲区，并返回拷贝的字节数。



//内部的rio_read函数
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
	int cnt;

	while(rp->rio_cnt <= 0){ /* Refill if buf is empty */
		rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, 
							sizeof(rp->rio_buf));
		if(rp->rio_cnt < 0){
			if(errno != EINTR) /* Interrupted by sig handler return */
				return -1;
		}
		else if(rp->rio_cnt == 0) //EOF
			return 0;
		else
			rp->rio_bufptr = rp->rio_buf; //Reset buffer ptr
	}

	/* Copy min(n, rp->rio_cnt) bytes from internal buf to user buf */
	cnt = n;
	if(rp->rio_cnt < n)
		cnt = rp->rio_cnt;
	memcpy(usrbuf, rp->rio_bufptr, cnt);
	rp->rio_bufptr += cnt;
	rp->rio_cnt -= cnt;
	return cnt;
}



//rio_readlineb和rio_readnb函数
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
	int n, rc;
	char c, *bufp = usrbuf;

	for(n = 1; n < maxlen; n++){
		if((rc = rio_read(rp, &c, 1)) == 1){
			*bufp++ = c;
			if(c == '\n')
				break;
		}else if(rc == 0){
			if(n == 1)
				return 0; /* EOF, no data read */
			else
				break; /* EOF, some data was read */
		}else
			return -1; /* error */
	}
	*bufp = 0;
	return n;
}


ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n)
{
	size_t nleft = n;
	ssize_t nread;
	char *bufp = usrbuf;

	while(nleft > 0){
		if((nread = rio_read(rp, bufp, nleft)) < 0){
			if(errno == EINTR)	/* Interrupted by sig handler return */
				nread = 0;		/* Call read() again */
			else
				return -1;		/* errno set by read() */
		}
		else if(nread == 0)
			break;				/* EOF */
		nleft -= nread;
		bufp += nread;
	}
	return (n - nleft);			/* Return >= 0 */
}






