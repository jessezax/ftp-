#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<signal.h>
#include<sys/select.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<sys/epoll.h>
#include<netdb.h>

#define FILENAME  "file"
typedef struct{
	int sfdbusy;
	int busy;
}business;
typedef struct{
	pid_t pid;
	int tfds;  //通过管道传递内核控制信息
	business b;
}child,*pchild;


typedef struct{
	int len;
	char buf[1000];
}train,*ptrain;

typedef struct{
	int len;
	char buf[502];
}train3g,*ptrain3g;

void send_data(int, char*);
void recv_data(int,char*);
void make_child(pchild ,int);
void send_fd(int ,int);
void recv_fd(int ,int*);
int send_n(int ,char *,int);
int recv_n(int ,char *,int);
void collect_include(int);
int LoginUser(char*,char*);
int RegisterUser(char*,char*);
int DeleteUser(char*);
void menu(int);
