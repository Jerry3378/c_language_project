#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<pthread.h>
#include<errno.h>

#define CLNT_MAX 10
#define BUFFSIZE 200

int g_clnt_socks[CLNT_MAX];		//for client-socket(global)
int g_clnt_count = 0;

void * clnt_connection(void *arg){

	int clnt_sock = *(int *)arg;
	int str_len = 0;

	char msg[BUFFSIZE];
	int i;

	while(1){

	str_len = read(clnt_sock, msg,sizeof(msg));		//클라이언트로 부터 온 메시지를 읽음

	if (str_len == -1)		//세션이 끊어진 상황
	{
		printf("clnt[%d] exit\n",clnt_sock);
		break;
	}

	printf("%s\n",msg);		//클라이언트로 부터 온 메시지를 뿌림
	

	}

	close(clnt_sock);		//소켓 닫음
	pthread_exit(0);
	return NULL;
}



int main(int argc, char ** argv){
	int serv_sock;		//server-socket
	int clnt_sock;		//client-socket

	pthread_t t_thread;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	memset(&serv_addr,0,sizeof(serv_addr));			//0으로 서버 어드레스 메모리 셋팅 
	memset(&clnt_addr,0,sizeof(clnt_addr));

	serv_sock = socket(PF_INET,SOCK_STREAM,0);		//PF_INET : connect to ipv4, SOCK_STREAM : TCP/IP type, 0 : protocol_num
	socklen_t clnt_addr_size;
	
	serv_addr.sin_family = AF_INET;

	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);		//host to network long == change the host's connection way into networks connections 

	serv_addr.sin_port = htons(4000);				//change host's order way into network way shorts port (16bits)

	if (bind(serv_sock,(const struct sockaddr *)&serv_addr,sizeof(serv_addr)) == -1){				//sizeof(serv_addr) == -1 이라고 써서 invalid argument라고 뜬 거였음
		printf("bind error\n");	
		printf("value of socket %d",serv_sock);	
		fprintf(stderr, "Error code: %d\n", errno);
		exit(-1);
	}

	if(listen(serv_sock,5) == -1){					//몇명을 listen상태로 대기 가능한가
		printf("listen error");
	}

	char buff[200];
	int recv_len = 0;						//얼마만큼 받아왔는가?

		clnt_addr_size = sizeof(clnt_addr);
		while(1){

		clnt_sock = accept(serv_sock,(struct sockaddr *)&clnt_addr,&clnt_addr_size);				//여기는 계속 손님을 받는 역활을 하는 곳임//

		pthread_create(&t_thread,NULL,clnt_connection,&clnt_sock);


	close(clnt_sock);	
	
	}


	printf("programe end\n");	
	return 0;


}
