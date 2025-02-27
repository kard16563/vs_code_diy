#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);
//./eserv 9190
// 有数据边界的udp套接字 tcp不存在数据边界  但udp 一个写对应一个读  不能一个读对应多个写 
// 必须保证 udp的io 次数一致
// udp 数据报 -》 udp传输的数据包 与tcp不同 其本身为一个完整的数据 这与 udp存在数据的边界有关  所以 1个数据包为1个完整的数据

int main(int argc, char *argv[])
{
	int sock;
	char message[BUF_SIZE];
	struct sockaddr_in my_adr, your_adr;
	socklen_t adr_sz;
	int str_len, i;

	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_DGRAM, 0);
	if(sock==-1)
		error_handling("socket() error");
	
	memset(&my_adr, 0, sizeof(my_adr));
	my_adr.sin_family=AF_INET;
	my_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	my_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(sock, (struct sockaddr*)&my_adr, sizeof(my_adr))==-1)
		error_handling("bind() error");
	
	for(i=0; i<3; i++) //读取3次
	{
		sleep(5);	// delay 5 sec. 推迟 5秒
		adr_sz=sizeof(your_adr);
		str_len=recvfrom(sock, message, BUF_SIZE, 0, 
								(struct sockaddr*)&your_adr, &adr_sz);     
	
		printf("Message %d: %s \n", i+1, message);
	}
	close(sock);	
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

/*
root@my_linux:/home/swyoon/tcpip# gcc bound_host1.c -o host1
root@my_linux:/home/swyoon/tcpip# ./host1 
Usage : ./host1 <port>
root@my_linux:/home/swyoon/tcpip# ./host1 9190
Message 1: Hi! 
Message 2: I'm another UDP host! 
Message 3: Nice to meet you 
root@my_linux:/home/swyoon/tcpip# 

*/
