#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sd, clnt_sd;
	FILE * fp;
	char buf[BUF_SIZE];
	int read_cnt;
	
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;
	
	if(argc!=2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	
	fp=fopen("file_server.c", "rb"); //	读取文件 
	serv_sd=socket(PF_INET, SOCK_STREAM, 0);   
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	bind(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
	listen(serv_sd, 5);
	
	clnt_adr_sz=sizeof(clnt_adr);    
	clnt_sd=accept(serv_sd, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
	
	while(1)
	{
		read_cnt=fread((void*)buf, 1, BUF_SIZE, fp);//fread( void *buffer, size_t size, size_t count, FILE *stream )从给定输入流stream读取最多count个对象到数组buffer中（相当于以对每个对象调用size次fgetc），把buffer当作unsigned char数组并顺序保存结果。流的文件位置指示器前进读取的字节数
		//返回成功读取的对象个数，若出现错误或到达文件末尾，则可能小于count

		//从给定流 fp 读取数据到 (void*)buf 所指向的数组中
		//(void*)buf -- 这是指向带有最小尺寸 size*nmemb 字节的内存块的指针。
		//1 -- 这是要读取的每个元素的大小，以字节为单位。
		//BUF_SIZE -- 这是元素的个数，每个元素的大小为 BUF_SIZE 字节。
		// fp -- 这是指向 FILE 对象的指针，该 FILE 对象指定了一个输入流。
		printf("\n ser read_cnt-> %d",read_cnt);
		if(read_cnt<BUF_SIZE)
		{
			write(clnt_sd, buf, read_cnt);
			printf("\n if ser read_cnt-> %d",read_cnt);
			break;
		}
		write(clnt_sd, buf, BUF_SIZE);
	}
	
	shutdown(clnt_sd, SHUT_WR);	//  clnt_sd 需要断开的套接字文件描述符 SHUT_WR 断开方式 -》 SHUT_WR：断开输出 SHUT_RD：断开输入 SHUT_DRWR：全部断开
	read(clnt_sd, buf, BUF_SIZE);
	printf("Message from client: %s \n", buf);// 关闭了输出流 但是可以继续 读
	
	fclose(fp);
	close(clnt_sd); close(serv_sd);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
