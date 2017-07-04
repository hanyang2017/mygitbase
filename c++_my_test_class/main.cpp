#include <iostream> 
#include "My_Test_Class.h"
#include "main.h"
#include <unistd.h>		//for sleep
#include <stdio.h>

#include <iostream>
#include <limits> // 为了使用numeric_limits

/***/
#include <arpa/inet.h>	//解决inet_addr was not declared in this scope报错
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define CLIENT_USE_PORT "7999"
#define SERVER_IP "192.168.155.100"

#define SERVER_USE_PORT 7998 
#define BUFFER_SIZE 512 
#define FILE_NAME_MAX_SIZE 512 
/***/

using std::cout;
using std::endl;
using std::cin;
using std::numeric_limits;
using std::streamsize;


void My_Test_Class::io_test( )                 
{
	// read till end-of-file, calculating a running total of all values read 
	#if 1
	while(1)
	{
		if(std::cin >> temp_val)
		{
			//用来打印出std::cin >> temp_val的值,但实际发现在这里打印后会导致求和结果不准
			//std::cout<<"cin return : "<<(std::cin >> temp_val)<<std::endl;
			sum += temp_val; // equivalent to sum = sum + temp_val 
		}
		else
		{			
			//用来打印出std::cin >> temp_val的值
			std::cout<<"cin return : "<<(std::cin >> temp_val)<<std::endl;
			break;
		}
	}
	
	#else
/* 	while (std::cin >> temp_val)
	{ 
		std::cout<<"input : "<<std::endl;
		sum += temp_val; // equivalent to sum = sum + temp_val 
		
	} */
	#endif
	std::cout << "Sum is: " << sum << std::endl; 
	sum=0;//成员函数直接引用对象，故在成员函数中赋给对象的值是可以在重入后保留下来的
/* 	std::cout<<"now test input abc in one row"<<std::endl;
	std::cin>>a>>b>>c;
	std::cout<<a<<b<<c<<std::endl;   */
}

void My_Test_Class::malloc_test( ) 
{
    char* buf_p=NULL;
    
    buf_p=(char *)malloc(100);//此处仅malloc的话，strlen结果仍是0
    printf("malloc , strlen buf_p =%d\n",strlen(buf_p));
    memset(buf_p,0,100);//malloc后，strlen仍是0
    printf("memset , strlen buf_p =%d\n",strlen(buf_p));
    strcpy(buf_p,"123456");//strcpy后，strlen才变成6，此处也是给了6个字符
    printf("strcpy , strlen buf_p =%d\n",strlen(buf_p));//综上，不管是malloc还是memset，都不能改变实际数组长度，而只有想strcpy这样的操作才能真正给数组写入内容，从而改变长度
}

void My_Test_Class::fread_test( ) 
{
    FILE*stream;
    char msg[]="this";
    char buf[20];
    int res=0;
    if((stream=fopen("/home/yh/myfiles/mycodes/c++_my_test_class/testfile.txt","w+"))==NULL)
    {
        printf("Can not file.\n");
    }
    printf("open file ok\n");
    /*write some data to the file*/
    //fwrite(msg,1,strlen(msg)+1,stream);
    /*sizeof（char）=1 seek to the beginning of the file*/
    //fseek(stream,0,SEEK_SET);
    /*read the data and display it*/
    res=fread(buf,1,strlen(msg)+1,stream);
    if(res==(strlen(msg)+1))
        printf("%s\n",buf);
    else
        printf("fread err\n");
    fclose(stream);
}

void p1_change_p2(void* p1,void* p2)
{
/* 	int temp=0;
	if((int *)p1 > (int *)p2)
	{
		temp = (int *)p1;
		*p1 = *p2;
		*p2 = temp;
	} */
	
}

/* void p1_change_p2(int* a,int* b)
{
	printf("\n");
} */
 
void My_Test_Class::point_test( ) 
{
	int a,b=0;
	int *point_1,*point_2;
	scanf("%d,%d",&a,&b);
	point_1 = &a;
	point_2 = &b;
	p1_change_p2(point_1,point_2);
	printf("a=%d  b=%d\n",a,b);
	
}

void My_Test_Class::array_test( ) 
{
    char aa[10]={0};
    printf("aa = %d\n",aa);
    printf("at first ,strlen aa =%d\n",strlen(aa));
    memset(aa,0,6);
    printf("after memset ,strlen aa =%d\n",strlen(aa));
    strcpy(aa,"123");
    printf("after strcpy ,strlen aa =%d\n",strlen(aa));
}

void My_Test_Class::udp_client_test( ) 
{
	printf("This is a UDP client\n");
    struct sockaddr_in addr;
    int sock;
	if ( (sock=socket(AF_INET, SOCK_DGRAM, 0)) <0)
    {
        printf("creat socket err\n");
        //exit(1);
    }
	printf("creat socket ok\n");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(CLIENT_USE_PORT));
    addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    if (addr.sin_addr.s_addr == INADDR_NONE)
    {
        printf("Incorrect ip address!\n");
        close(sock);
        //exit(1);
    }
	char buff[512];
    int len = sizeof(addr);
	printf("begin to tx/rx\n");
    while (1)
    {
        memset(buff,0,512);
		strcpy(buff,"GET_HG_PARAM_VALUE   FULL_PATH=InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService ");
		//buff_encode=base64_encode(buff);
        int n;
        printf("sendto start...\n");
        //n = sendto(sock, buff_encode, strlen(buff_encode), 0, (struct sockaddr *)&addr, sizeof(addr));
		n = sendto(sock, buff, strlen(buff), 0, (struct sockaddr *)&addr, sizeof(addr));
        if (n < 0)
        {
            printf("sendto server \n");
            close(sock);
            break;
        }
        printf("sendto done...\n");
		#if 0//测试udp socket在对端关闭后会否select返回-1
		fd_set sockset;
		FD_ZERO(&sockset);
		FD_SET(sock, &sockset);
		int retaaa=0;
		struct timeval timeout={3,0}; //select等待3秒，3秒轮询，要非阻塞就置0 
		retaaa = select(sock+1, &sockset, NULL, NULL, &timeout);
		printf("retaaa=%d\n",retaaa);
		#endif
        memset(buff,0,512);
        //n = recvfrom(sock, buff, 512, 0, (struct sockaddr *)&addr, &len);
		n = recvfrom(sock, buff, 512, 0, (struct sockaddr *)&addr, (socklen_t *)&len);
        printf("recvfrom done...n=%d\n",n);
        if (n>0)
        {
            //buff_decode=base64_decode(buff);
            printf("received:\n");
            //puts(buff_decode);
			puts(buff);
            char test_value[512];
            memset(test_value,0,512);
            //sscanf(buff_decode,"VALUE=%s",test_value);
			sscanf(buff,"VALUE=%s",test_value);
            printf("test_value=%s\n",test_value);
			
        }
        else if (n==0)
        {
            printf("server closed\n");
            close(sock);
            break;
        }
        else if (n == -1)
        {
            printf("recvfrom err\n");
            close(sock);
            break;
        }

   } 
}

void My_Test_Class::udp_server_test( ) 
{
	 printf("this is a udp server\n");
	/* 创建UDP套接口 */
	 struct sockaddr_in server_addr; 
	 bzero(&server_addr, sizeof(server_addr)); 
	 /*AF_INET（又称 PF_INET）是 IPv4 网络协议的套接字类型，AF_INET6 则是 IPv6 的；而 AF_UNIX 则是 Unix 系统本地通信。*/
	 server_addr.sin_family = AF_INET; 
	 server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	 server_addr.sin_port = htons(SERVER_USE_PORT); 
	 /* 创建socket */
	 int server_socket_fd = socket(AF_INET, SOCK_DGRAM, 0); 
	 if(server_socket_fd == -1) 
	 { 
	  printf("Create Socket err\n"); 
	  //exit(1); 
	 } 
	  printf("Create Socket ok\n"); 
	 /* 绑定套接口 */
	 if(-1 == (bind(server_socket_fd,(struct sockaddr*)&server_addr,sizeof(server_addr)))) 
	 { 
	  printf("Server Bind err\n"); 
	  //exit(1); 
	 } 
   /* 数据传输 */
	 while(1) 
	 {  
	  /* 定义一个地址，用于捕获客户端地址 */
	  struct sockaddr_in client_addr; 
	  socklen_t client_addr_length = sizeof(client_addr); 
	  
	  /* 接收数据 */
	  char buffer[BUFFER_SIZE]; 
	  bzero(buffer, BUFFER_SIZE); 
	  if(recvfrom(server_socket_fd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&client_addr, &client_addr_length) == -1) 
	  { 
	   printf("Receive Data err\n"); 
	   //exit(1); 
	  } 
	  
	  /* 从buffer中拷贝出file_name */
	  //char file_name[FILE_NAME_MAX_SIZE+1]; 
	  //bzero(file_name,FILE_NAME_MAX_SIZE+1); 
	  //strncpy(file_name, buffer, strlen(buffer)>FILE_NAME_MAX_SIZE?FILE_NAME_MAX_SIZE:strlen(buffer)); 
	  //printf("%s\n", file_name); 
		printf("%s\n", buffer); 
		memset(buffer,0,BUFFER_SIZE);
		strcpy(buffer,"server send 123");
		int n;
		n=sendto(server_socket_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, client_addr_length);
		if (n < 0)
		{
			printf("server send back err\n");
			close(server_socket_fd);
			return;
		}

	 } 
	 close(server_socket_fd); 
	 return; 
}

int main() 
{ 
	My_Test_Class test1;
	test1.op_code=0;
	test1.temp_val=0;
	test1.sum=0;
	while(1)
	{
		std::cout<<"please input op code"<<std::endl;
		std::cout<<"1:CASE_IO_TEST  2:CASE_POINT_TEST  3:CASE_SSCANF_TEST  4:CASE_SPRINTF_TEST  5:CASE_STRSTR  6:CASE_UDP_CLIENT_TEST  7:CASE_UDP_SERVER_TEST  8:CASE_MALLOC_TEST  9:CASE_FREAD_TEST  10:CASE_ARRAY_TEST"<<std::endl;
		std::cin>>test1.op_code;
		std::cout<<"chose op code "<<test1.op_code<<std::endl;
		//sleep(3);
		switch(test1.op_code)
		{
			case CASE_IO_TEST:
				std::cout<<"enter case CASE_IO_TEST"<<std::endl;
				test1.io_test();
				break;
			case CASE_POINT_TEST:
				std::cout<<"enter case CASE_POINT_TEST"<<std::endl;
				test1.point_test();
				break;
			case CASE_SSCANF_TEST:
				std::cout<<"enter case CASE_SSCANF_TEST"<<std::endl;
				
				break;
			case CASE_SPRINTF_TEST:
				std::cout<<"enter case CASE_SPRINTF_TEST"<<std::endl;
				
				break;
			case CASE_STRSTR:
				std::cout<<"enter case CASE_STRSTR"<<std::endl;
				
				break;
			case CASE_UDP_CLIENT_TEST:
				std::cout<<"enter case CASE_UDP_CLIENT_TEST"<<std::endl;
				test1.udp_client_test();
				break;
			case CASE_UDP_SERVER_TEST:
				std::cout<<"enter case CASE_UDP_SERVER_TEST"<<std::endl;
				test1.udp_server_test();
				break;		
			case CASE_MALLOC_TEST:
				std::cout<<"enter case CASE_MALLOC_TEST"<<std::endl;
				test1.malloc_test();
				break;   
			case CASE_FREAD_TEST:
				std::cout<<"enter case CASE_FREAD_TEST"<<std::endl;
				test1.fread_test();
				break;     
			case CASE_ARRAY_TEST:
				std::cout<<"enter case CASE_ARRAY_TEST"<<std::endl;
				test1.array_test();
				break;                 
			default :
				std::cout<<"no compare option"<<std::endl;
				break;
			
		}
		//fflush(std::cin);
		std::cin.clear();
		std::cin.ignore( numeric_limits<streamsize>::max(), '\n' );
		continue;
		
	}

	return 0; 
} 
