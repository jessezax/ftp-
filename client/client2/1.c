 ///
 /// @file    1.c
 /// @author  jesse(821228705@qq.com)
 /// @date    2017-09-17 15:59:26
 ///
 
#include <unistd.h>
int main(int argc,char**argv)
{
     /* 
	  *      * 执行/bin目录下的ls,第一参数为程序名ls, 
	  *           * 第二个参数为-al,第三个参数为/etc/ 
	  *                */  
   
	if(fork())
	{
	execl("./client","client",argv[1],argv[2],NULL);  
	}
	execl("./client","client",argv[1],argv[2],NULL);  
	       
	     return 0;  
}
