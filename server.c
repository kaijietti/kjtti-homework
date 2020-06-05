/*
文件：server.c
PS：第一个连接上服务器的客户端，称为client1，第二个连接上服务器的客户端称为client2
这个服务器的功能是：
1：对于client1，它返回"first"，并在client2连接上之后，将client2经过转换后的IP和port发给client1；
2：对于client2，它返回client1经过转换后的IP和port和自身的port，并在随后断开与他们的连接。
 */
 
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
 
#define MAXLINE 128
#define SERV_PORT 8877
 
//发生了致命错误，退出程序
 
void error_quit(const char *str) {
	fprintf(stderr, "%s", str);
	//如果设置了错误号，就输入出错原因
	if (errno != 0)
		fprintf(stderr, " : %s", strerror(errno));
	printf("\n");
	exit(1);
}
 
int main(void) {
	int res, cur_port;
	int connfd, firstfd, listenfd;
	int count = 0;
	char str_ip[MAXLINE] = {0}; 	// current ip
	char cur_inf[MAXLINE] = {0}; 	// current connection info [IP+port]
	char str_ip1[MAXLINE] = {0}; 	// first client's ip
	int aport = 0;			// first client's port
	char first_inf[MAXLINE] = {0}; 	// first connetion info [IP+port]
	char buffer[MAXLINE] = {0}; 	// buffer to send/recv messages

	struct sockaddr_in cliaddr;
	struct sockaddr_in servaddr;
	socklen_t clilen;
	
	memset(&cliaddr, 0, sizeof (struct sockaddr_in));
	clilen = (socklen_t)sizeof (cliaddr);
 
	//创建用于监听UDP协议套接字        
	listenfd = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&servaddr, 0, sizeof (servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	//把socket和socket地址结构联系起来       
	res = bind(listenfd, (struct sockaddr *) &servaddr, sizeof (servaddr));
	if (-1 == res)
		error_quit("bind error");

	while (1) {
		printf("waiting\n");
		fflush(stdout);
		//接收来自客户端的连接
 
		struct sockaddr_in addr, addr1;

		memset(&addr, 0, sizeof (struct sockaddr_in));
		socklen_t addrlen = 0;
		addrlen = sizeof (struct sockaddr_in);
		recvfrom(listenfd, buffer, MAXLINE, 0, (struct sockaddr*) & addr, &addrlen);
		// addr means 'from'
		count++;
		inet_ntop(AF_INET, (void*) &addr.sin_addr, str_ip, sizeof (str_ip));
		// means number(addr.sin_addr) -> presentation(str_ip)
		cur_port = ntohs(addr.sin_port);
		// host-byte-order(network to host)


		// this point means allready get client's ip and port [(addr.sin_addr, str_ip), (addr.sin_port, cur_port)];
		if (count == 1) {
			printf("accept %s\n", buffer);			// show message from first client
			printf("accept1:  %s %d\n", str_ip, cur_port);	// show first connection info [IP+port]
			fflush(stdout);
			firstfd = connfd;				// set firstfd
			snprintf(first_inf, MAXLINE, "%s %d", str_ip, cur_port);// set first_inf
			aport = cur_port;				// set first_port
			strcpy(str_ip1, str_ip);			
			strcpy(cur_inf, "first\n");
			sendto(listenfd, cur_inf, strlen(cur_inf), 0, (struct sockaddr *) &addr, sizeof (struct sockaddr_in)); 
			//向客户端1发送first 告诉你是第一个
			if (-1 == res)
				error_quit("sendto error");
 			// set addr1 so that it can used by second connection
			// so addr1 actucally is first client
			memset(&addr1, 0, sizeof (struct sockaddr_in));			
			inet_pton(AF_INET, str_ip, &addr1.sin_addr);
			addr1.sin_port = htons(cur_port);
 
		} else if (count == 2) {
			printf("accept %s\n", buffer);
			printf("accept2:  %s %d\n", str_ip, cur_port);
			fflush(stdout);
			//cur_port = ntohs(cliaddr.sin_port);
			snprintf(cur_inf, MAXLINE, "%s %d\n", str_ip, cur_port);
			snprintf(buffer, MAXLINE, "%s %d\n", str_ip1, aport);

			sendto(listenfd, buffer, strlen(buffer), 0, (struct sockaddr *) &addr, sizeof (struct sockaddr_in)); 
			//send first client's info to second client
			if (-1 == res)
				error_quit("sendto error");
			sendto(listenfd, cur_inf, strlen(cur_inf), 0, (struct sockaddr *) &addr1, sizeof (struct sockaddr_in)); 			//send second client's info to first client
			if (-1 == res)
				error_quit("sendto2 error");

			sleep(5);
			printf("close\n");
			fflush(stdout);
			close(listenfd);
			return 0;
		} else
			error_quit("Bad required");
	}
	close(listenfd);
	return 0;
}
