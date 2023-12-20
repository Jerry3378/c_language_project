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
    struct sockaddr_in serv_addr;           //ipv4������ server address ����

    pthread_t snd_thread, rcv_thread;           //send thread�� recieve thread ����

    void *thread_result;

    sock = socket(PF_INET,SOCK_STREAM,0);           //ipv4������ tcp/ip������ϴ� ������ ������ ����

    if (sock == -1)
    {
        printf("socket() error");
    }
        memset(&serv_addr,0,sizeof(serv_addr));             //serv_addr�� 0���� �޸� �ʱ�ȭ

        serv_addr.sin_family = PF_INET;                 //tcp, ip4�������� ����
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");     //127.0.0.1�� ���� (���� pc�����δ� 127.0.0.1��)
        serv_addr.sin_port = htons(4000);                //��Ʈ ��ȣ�� 4000���� ����

        if (connect(sock, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) == -1)     //��������
        {
            printf("connect error");
        }

        unsigned char msg [100] = {0x01,2,3,4,5,6,1,2,3,4,2,1,2,3,0x0c};
        
        while(1){

            printf("send : ");
            write(sock,msg,15);                                 //msg������ ����
            sleep(1);

        }

        close(sock);
        return 0;
    
}
