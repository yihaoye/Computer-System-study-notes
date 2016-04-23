/* I/O of system, 如何打开文件读写从而读写I/O */
/* 与windows不同，Linux操作系统都是基于文件概念的（这个很很重要啊），文件是以字符序列构成的信息载体。
根据这一点，可以把I/O设备（网络、磁盘、终端等）当做文件来处理。因此，与磁盘上的普通文件进行交互所用的同一系统调用可以直接用于I/O设备。
这样大大简化了系统对不同设备的处理，提高了效率。 */
//http://webcache.googleusercontent.com/search?q=cache:XJftPv58quMJ:blog.csdn.net/mybelief321/article/details/8989755+&cd=2&hl=en&ct=clnk&gl=nz

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define DEF_MODE S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH
#define DEF_UMASK S_IWGRP|S_IWOTH

int open(char *pathname, int flags, mode_t mode); //返回：若成功则为新文件描述符（返回的描述符总是在进程中当前没有打开的最小描述符数字），若出错为－1
//open是UNIX系统（包括LINUX、Mac等）的系统调用函数，pathname参数是待打开/创建文件的POSIX路径名（如/home/user/a.cpp），
//flag参数指明如何访问该文件，mode参数指定新文件的访问权限，mode参数仅在创建新文件时使用（即使用了O_CREAT时）


//example:
fd = Open("foo.txt", O_RDONLY);
fd = Open("foo.txt", O_WRONLY | O_APPEND); //打开一个已存在文件，只写操作，在文件结尾处写信息


umask(DEF_UMASK); //每个进程都有个umask，用来设置访问权限，创建一个文件总是有一个默认权限
fd = Open("foo.txt", O_CREAT|O_TRUNC|O_WRONLY, DEF_MODE); //文件访问权限为"mode & ~umask"

/* 访问权限位，在sys/stat.h 中定义

S_IRUSR		使用者能够读此文件
S_IWUSR		使用者能够写此文件
S_IXUSR 	使用者能够执行此文件

S_IRGRP		使用者所在组成员能够读此文件
S_IWGRP		使用者所在组成员能够写此文件
S_IXGRP 	使用者所在组成员能够执行此文件

S_IROTH		任何人能够读此文件
S_IWOTH		任何人能够写此文件
S_IXOTH 	任何人能够执行此文件

*/



//读、写、关闭打开的I/O文件
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t n); //返回：若成功则为读的字节数，若EOF则为0，若出错为－1
ssize_t write(int fd, const void *buf, size_t n); //返回：若成功则为写的字节数，若出错为－1

int close(int fd); //返回：若成功则为0，若出错为－1





