#include "utility.h"

// struct connection
// {
//     int fd;
//     struct sockaddr_in address;
//     int error;
//     bool Successfully;
// };

static int clients[100];
static int cnt = 0;

static void accept_connection(int listenfd);
static int accept_socket(int listenfd);
static void create_thread(int clientfd);
static void* function(void* arg);
static void broadcast(char *buf, int clientfd);


static void broadcast(char *buf, int clientfd)
{
    for(int i = 0; i < cnt; i++)
    {
        if(clients[i] != clientfd)
        {
            send(clients[i],buf,strlen(buf),0);
        }
    }
}


static void create_thread(int clientfd)
{
    pthread_t id;
    pthread_create(&id,NULL,function,(void*)(long)clientfd);
}

static void* function(void* arg) 
{
    long clientfd = (long)arg;
    char buf[1024] = {'\0'};
    while(true)
    {
        ssize_t len = recv(clientfd,buf,1024,0);
        if(len > 0)
        {
            buf[len] = '\0';
            printf("%s\n",buf);
            broadcast(buf,clientfd); 
        }else if(len == 0){
            printf("client%ld is leaving\n",clientfd);
            break;
        }else if(len < 0)
        {
            perror("recv error");
            break;
        }
    }
    close(clientfd);
    return NULL;
}


static void accept_connection(int listenfd) 
{
    while(true)
    {
        int clientfd = accept_socket(listenfd);
        if(clientfd > 0)
        {
            clients[cnt++] = clientfd;
            create_thread(clientfd);
        }
    }
}

static int accept_socket(int listenfd)
{
    struct sockaddr_in clientaddr;
    socklen_t sockaddr_len = sizeof(clientaddr);
    int clientfd = accept(listenfd,(struct sockaddr*)(&clientaddr),&sockaddr_len);
    return clientfd;
}


int main()
{
    int listenfd = create_socket();
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    make_address(&address,"",4000);
    int rc = bind(listenfd,(struct sockaddr*)(&address),sizeof(struct sockaddr_in));
    if(rc < 0)
    {
        perror("bind error\n");
        close(listenfd);
        exit(1);
    }
    rc = listen(listenfd,10);
    if(rc < 0)
    {
        perror("listen error\n");
        close(listenfd);
        exit(1);
    }
    accept_connection(listenfd);
    close(listenfd);
    return 0;
}