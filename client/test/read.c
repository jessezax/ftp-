 ///
 /// @file    read.c
 /// @author  jesse(821228705@qq.com)
 /// @date    2017-09-07 14:27:07
 ///
 
#include <stdio.h>
#include <strings.h>
void collect_show()
{
	char buf[1000]={0};
	FILE *fp=fopen("collection.txt","rb+");
	bzero(buf,sizeof(buf));
	while(bzero(buf,sizeof(buf)),fscanf(fp,"%s",buf)!=EOF)
	{
		printf("%s ",buf);
		bzero(buf,sizeof(buf));
		fscanf(fp,"%s",buf);
		printf("%s\n",buf);
	}
	//send_n(sfd,buf,strlen(buf));
}

int main()
{
	collect_show();
}
