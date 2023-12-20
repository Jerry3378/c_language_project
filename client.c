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

    pthread_t snd_thread, rcv_thread;           //send thread와 recieve thread 설정

    void *thread_result;

    sock = socket(PF_INET,SOCK_STREAM,0);           //ipv4형식의 tcp/ip통신을하는 소켓을 구멍을 만듦

    if (sock == -1)
    {
        printf("socket() error");
    }
        memset(&serv_addr,0,sizeof(serv_addr));             //serv_addr을 0으로 메모리 초기화

        serv_addr.sin_family = PF_INET;                 //tcp, ip4형식으로 접속
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");     //127.0.0.1로 접속 (같은 pc상으로는 127.0.0.1임)
        serv_addr.sin_port = htons(4000);                //포트 번호는 4000으로 했음

        if (connect(sock, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) == -1)     //연결했음
        {
            printf("connect error");
        }

        unsigned char msg [100] = {0x01,2,3,4,5,6,1,2,3,4,2,1,2,3,0x0c};
        
        while(1){

            printf("send : ");
            write(sock,msg,15);                                 //msg내용을 보냄
            sleep(1);

        }

        close(sock);
        return 0;
    
}
