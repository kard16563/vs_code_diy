#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	unsigned short host_port=0x1234;
	unsigned short net_port;
	unsigned long host_addr=0x12345678;
	unsigned long net_addr;
	
	net_port=htons(host_port);
	net_addr=htonl(host_addr);
	
	printf("主机顺序 端口Host ordered port: %#x \n", host_port);
	printf("网络顺序 端口Network ordered port: %#x \n", net_port);
	printf("主机顺序 地址Host ordered address: %#lx \n", host_addr);
	printf("网络顺序 地址Network ordered address: %#lx \n", net_addr);
	
	return 0;
}
