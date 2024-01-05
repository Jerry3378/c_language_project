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

pthread_mutex_t g_mutex;

int g_clnt_socks[CLNT_MAX];		//for client-socket(global)
int g_clnt_count = 0;

void * clnt_connection(void *arg){

	int clnt_sock = *(int *)arg;
	int str_len = 0;

	char msg[BUFFSIZE];
	int i;

	while(1){

	str_len = read(clnt_sock, msg,sizeof(msg));		//?��?��?��?��?���? �??�� ?�� 메시�?�? ?��?��

	if (str_len == -1)		//?��?��?�� ?��?���? ?��?��
	{
		perror("clnt error : \n");
		printf("clnt[%d] exit\n",clnt_sock);
		break;
	}

	printf("%s\n",msg);		//?��?��?��?��?���? �??�� ?�� 메시�?�? 뿌림
	
	pthread_mutex_lock(&g_mutex);

	for (int i=0;i<g_clnt_count;i++){ 
		if(g_clnt_socks[i] != clnt_sock){				//accept? ????? ?? ?? ??? ??? ????? ??? 
			printf("sned : %s\n",msg);
			write(g_clnt_socks[i],msg,strlen(msg)+1);
			}
		}
	pthread_mutex_unlock(&g_mutex);
	
		
	}

	close(clnt_sock);		//?���? ?��?��
	pthread_exit(0);
	return NULL;
}



int main(int argc, char ** argv){
	int serv_sock;		//server-socket
	int clnt_sock;		//client-socket

	int my_sock;		//?????? ??? ?? ??? ?? ?????

	pthread_t t_thread;
	
	pthread_mutex_init(&g_mutex,NULL);

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	memset(&serv_addr,0,sizeof(serv_addr));			//0?���? ?���? ?��?��?��?�� 메모�? ?��?�� 
	memset(&clnt_addr,0,sizeof(clnt_addr));

	serv_sock = socket(PF_INET,SOCK_STREAM,0);		//PF_INET : connect to ipv4, SOCK_STREAM : TCP/IP type, 0 : protocol_num
	socklen_t clnt_addr_size;
	
	serv_addr.sin_family = AF_INET;

	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);		//host to network long == change the host's connection way into networks connections 

	serv_addr.sin_port = htons(4000);				//change host's order way into network way shorts port (16bits)

	if (bind(serv_sock,(const struct sockaddr *)&serv_addr,sizeof(serv_addr)) == -1){				//sizeof(serv_addr) == -1 ?��?���? ?��?�� invalid argument?���? ?�� 거�???��
		printf("bind error\n");	
		printf("value of socket %d",serv_sock);	
		fprintf(stderr, "Error code: %d\n", errno);
		exit(-1);
	}

	if(listen(serv_sock,5) == -1){					//몇명?�� listen?��?���? ???�? �??��?���?
		printf("listen error");
	}

	char buff[200];
	int recv_len = 0;						//?��마만?�� 받아?��?���??

		clnt_addr_size = sizeof(clnt_addr);
	while(1){

		clnt_sock = accept(serv_sock,(struct sockaddr *)&clnt_addr,&clnt_addr_size);				//?��기는 계속 ?��?��?�� 받는 ?��?��?�� ?��?�� 곳임//

		pthread_mutex_lock(&g_mutex);
		g_clnt_socks[g_clnt_count++] = clnt_sock;		//accept�� Ŭ���̾�Ʈ���� ��ũ���ǰ��� ����
		pthread_mutex_unlock(&g_mutex);

		pthread_create(&t_thread,NULL,clnt_connection,&clnt_sock);
	
	}

	
	close(clnt_sock);	
	printf("programe end\n");	
	return 0;


}

