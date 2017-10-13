 ///
 /// @file    main.c
 /// @author  jesse(821228705@qq.com)
 /// @date    2017-09-04 12:23:10
 ///
 //

#include "func.h"
int fds_exit[2];

void sig(int signum)
{
	char flag='h';
	printf("server exit:%d\n",signum);
	write(fds_exit[1],&flag,sizeof(flag));
}

int main(int argc,char * argv[])
{
	if(argc!=4)
	{
		perror("args\n");
		return -1;
	}
	signal(SIGINT,sig);
	pipe(fds_exit);
	int child_num=atoi(argv[3]);
	pchild p=(pchild)calloc(child_num,sizeof(child));
	make_child(p,child_num); //创建子进程并且初始化有数据结构
	int sfd;
	sfd = socket(AF_INET,SOCK_STREAM,0);//ipv4,TCP
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in ser;
	memset(&ser,0,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2])); //小端
	ser.sin_addr.s_addr=inet_addr(argv[1]);
	int ret;
	ret=bind(sfd,(struct sockaddr*)&ser,sizeof(ser));
	if(-1==ret)
	{
		perror("bind");
		return -1;
	}
	int epfd=epoll_create(1);
	struct epoll_event event,*evs;
	evs=(struct epoll_event*)calloc(child_num+2,sizeof(struct epoll_event));
	memset(&event,0,sizeof(event));
	event.events=EPOLLIN;//读
	event.data.fd=sfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);
	event.events=EPOLLIN;//读
	event.data.fd=fds_exit[0];
    epoll_ctl(epfd,EPOLL_CTL_ADD,fds_exit[0],&event);
	int i;
	for(i=0;i<child_num;i++)  //父进程监控每一个tfds
	{
		event.events=EPOLLIN;
		event.data.fd=p[i].tfds;
		epoll_ctl(epfd,EPOLL_CTL_ADD,p[i].tfds,&event);
	}
	listen(sfd,child_num+2); //端口打开
	int ret1;
	int new_fd;
	int j;
	char flag;
	struct sockaddr_in clientaddr;
	memset(&clientaddr,0,sizeof(struct sockaddr));
	socklen_t addrlen=sizeof(struct sockaddr);
	while(1)
	{
		//memset(evs,0,sizeof(*evs));
		ret1=epoll_wait(epfd,evs,child_num+2,-1);
		for(i=0;i<ret1;i++)
		{
			if(evs[i].data.fd==sfd)//网络到达
			{
				//new_fd=accept(sfd,NULL,NULL);
				new_fd=accept(sfd,(struct sockaddr*)&clientaddr,&addrlen);
				printf("%s connect success\n",inet_ntoa(clientaddr.sin_addr));
				//printf("accept :%d\n",new_fd);
				//找非忙碌子进程
				for(j=0;j<child_num;j++)
				{
					if(p[j].b.sfdbusy==0)
					{
						send_fd(p[j].tfds,new_fd);//描述符发给子进程
						p[j].b.sfdbusy=1;
						p[j].b.busy=1;
						printf("find a not busy process,send fd success\n");
						break;
					}
				}
			    close(new_fd);//引用计数交给子进程
			}
			for(j=0;j<child_num;j++)
			{
				if(evs[i].data.fd==p[j].tfds)
				{
					read(p[j].tfds,&flag,sizeof(flag));
					p[j].b.busy=0;
					//printf("child task is free\n");
				}
			}
			if(evs[i].data.fd==fds_exit[0])
			{
				read(fds_exit[0],&flag,sizeof(flag));
				epoll_ctl(epfd,EPOLL_CTL_DEL,sfd,&event);
				close(sfd);
				for(i=0;i<child_num;i++)
				{
					while(p[i].b.busy)
					{
						read(p[i].tfds,&flag,sizeof(flag));
						p[i].b.busy=0;
					}
					event.events=EPOLLIN;
					event.data.fd=p[i].tfds;
					epoll_ctl(epfd,EPOLL_CTL_DEL,p[i].tfds,&event);
					close(p[i].tfds);
				}
				kill(0,SIGKILL);
			}
		}
	}
	return 0;
}
