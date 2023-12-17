#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<pthread.h>

#define CLNT_MAX 10

int g_clnt_socks[CLNT_MAX];		//for client-socket(global)


int main(int argc, char ** argv){
	int serv_sock;		//server-socket
	int clnt_sock;		//client-socket

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;


	serv_sock = socket(PF_INET,SOCK_STREAM,0);		//PF_INET : connect to ipv4, SOCK_STREAM : TCP/IP type, 0 : protocol_num
	socklen_t clnt_addr_size;
	
	serv_addr.sin_family = AF_INET;

	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);		//host to network long == change the host's connection way into networks connections ip

	serv_addr.sin_port = htons(7889);				//change host's order way into network way shorts port (16bits)

	if (bind(serv_sock,&serv_addr,sizeof(serv_addr) == -1)){
		printf("bind error\n");
	}

	if(listen(serv_sock,5) == -1){					//몇명을 listen상태로 대기 가능한가
		printf("listen error");
	}

	while(1){
		clnt_addr_size = sizeof(clnt_addr);

		clnt_sock = accept(serv_sock,&clnt_addr,&clnt_addr_size);
	}


}
