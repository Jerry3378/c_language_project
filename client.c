#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUFSIZE 100
#define NAMESIZE 20

int StoHEX(char fi, char sc);
void *send_message(void *arg);
void *recv_message(void *arg);

char messagep[BUFSIZE];

int main(int argc, char **argv)
{
    int sock;
    struct sockaddr_in serv_addr;           //ipv4형식의 server address 설정

    int i = 0;
    char id[100];
    char chat[100] = {0};


    if (argc < 2){
        printf("id를 입력하시오\n");
    }

    strcpy(id,argv[1]);

    pthread_t snd_thread, rcv_thread;           //send thread와 recieve thread 설정

    void *thread_result;

    sock = socket(AF_INET,SOCK_STREAM,0);           //ipv4형식의 tcp/ip통신을하는 소켓을 구멍을 만듦

    if(sock == -1){
	    perror("socket error : ");
    } 


        memset(&serv_addr,0,sizeof(serv_addr));             //serv_addr을 0으로 메모리 초기화

        serv_addr.sin_family = PF_INET;                 //tcp, ip4형식으로 접속
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");     //127.0.0.1로 접속 (같은 pc상으로는 127.0.0.1임)
        serv_addr.sin_port = htons(4000);                //포트 번호는 4000으로 했음

        if (connect(sock, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) == -1)     //연결했음 연결이 실패 할 경우 -1을 반환
        {
            perror("error code : ");
        }

        char msg [1000];
        
        printf("while before\n");
        while(1){

	    printf("chat : ");
            fgets(chat,sizeof(chat),stdin);

            printf("send : %s\n",chat);
            sprintf(msg,"[%s] : %s",id,chat);
            printf("[%s] : %s\n",id,chat);
            if (write(sock,msg,strlen(msg)+1) == -1)               //msg내용을 보냄
                perror("write : ");

            sleep(1);

        }
        printf("while end");

        close(sock);
        return 0;
    
}

