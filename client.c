#include "utility.h"

static void listen_thread(int fd);
static void write_thread(int fd);
static void* function(void *arg);


static void* function(void *arg)
{
    long fd = (long)arg;
    char buffer[1024];
    while (true)
    {
        memset(buffer,0,1024);
        ssize_t  len = recv(fd,buffer,1024,0);
        if(len > 0)
        {
            printf("%s\n",buffer);
        }
        if(len == 0)
            break;
    }
}

static void listen_thread(int fd) 
{
    pthread_t id ;
    pthread_create(&id,NULL,function,(void*)(long)fd);
    return;
}

static void write_thread(int fd)
{
    char *name = NULL;
    char *line = NULL;
    size_t name_size = 0;
    size_t line_size = 0;
    char buffer[1024];

    printf("please enter your name?\n");
    ssize_t k = getline(&name,&name_size,stdin);
    name[k-1] = 0;    // disappear the delimiter character

    printf("<<<---start chating--->>>(type exit to leave)\n");
    while(true)
    {   
        ssize_t len = getline(&line,&line_size,stdin);
        if(len < 0)
        {
            perror("getline error\n");
            break;
        }    
        line[len-1] = 0;
        if(strcmp(line,"exit") == 0)
        {
            printf("exit\n");
            break;
        }else{
            sprintf(buffer,"%s:%s",name,line);
            send(fd,buffer,strlen(buffer),0);
        }
    }
    free(name);
    free(line);
    return;
}

int main()
{
    int fd = create_socket();
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    make_address(&address,"127.0.0.1",4000);
    int rc = connect(fd,(struct sockaddr*)(&address),sizeof(struct sockaddr_in));
    if(rc < 0)
    {
        close(fd);
        perror("connect error\n");
        exit(1);
    }
    listen_thread(fd);
    write_thread(fd);
    close(fd);
    return 0;
}