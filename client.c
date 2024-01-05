#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>

#define BUFSIZE 100
#define NAMESIZE 20

pthread_mutex_t g_mutex;

void *recv_message(void *arg);

void *recv_message(void *arg){
    int str_len = 0;
    char recv_msg[100];

    pthread_mutex_init(&g_mutex,NULL);              //mutex 초기화


    int srv_sock = *(int *)arg;
    
    //서버로부터 받은 문장을 출력해줌//
    while(1){
    str_len = read(srv_sock,recv_msg,sizeof(recv_message));


    if (str_len != -1)
    {
        pthread_mutex_lock(&g_mutex);    
        printf("%s",recv_msg);
        fflush(stdout);
        pthread_mutex_unlock(&g_mutex);
    }

    }
    
    pthread_exit(NULL);
    return NULL;

}

void *send_message(void *arg){}

char messagep[BUFSIZE];

int main(int argc, char **argv)
{
    int sock;
    int srv_sock;
    struct sockaddr_in serv_addr;           //ipv4?삎?떇?쓽 server address ?꽕?젙

    int i = 0;
    char id[100];
    char chat[100] = {0};


    if (argc < 2){
        printf("id를 입력하시오 \n");
    }

    strcpy(id,argv[1]);

    pthread_t rcv_thread;           //send thread??? recieve thread ?꽕?젙

    void *thread_result;

    sock = socket(AF_INET,SOCK_STREAM,0);           //ipv4?삎?떇?쓽 tcp/ip?넻?떊?쓣?븯?뒗 ?냼耳볦쓣 援щ찉?쓣 留뚮벀

    if(sock == -1){
	    perror("socket error : ");
    } 

        pthread_create(&rcv_thread,NULL,recv_message,&sock);
        printf("recv thread created\n");
        
        
        memset(&serv_addr,0,sizeof(serv_addr));             //serv_addr?쓣 0?쑝濡? 硫붾え由? 珥덇린?솕

        serv_addr.sin_family = PF_INET;                 //tcp, ip4?삎?떇?쑝濡? ?젒?냽
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");     //127.0.0.1濡? ?젒?냽 (媛숈?? pc?긽?쑝濡쒕뒗 127.0.0.1?엫)
        serv_addr.sin_port = htons(4000);                //?룷?듃 踰덊샇?뒗 4000?쑝濡? ?뻽?쓬

        if (srv_sock = connect(sock, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) == -1)     //?뿰寃고뻽?쓬 ?뿰寃곗씠 ?떎?뙣 ?븷 寃쎌슦 -1?쓣 諛섑솚
        {
            perror("error code : ");
        }

        char msg [1000];
        


        printf("while before\n");
        
        while(1){

            printf("[%s] : ",id);
            fgets(chat,sizeof(chat),stdin);
            sprintf(msg,"[%s] : %s",id,chat);
            
            if (write(sock,msg,strlen(msg)+1) == -1)               //msg?궡?슜?쓣 蹂대깂
                perror("write : ");
 

            sleep(1);

        }

        close(sock);
        printf("while end");

        return 0;
    
}