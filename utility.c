#include "utility.h"

int create_socket()
{
    int yes = 1;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(1);
    }
    // 设置 SO_REUSEADDR 选项
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("setsockopt");
        exit(1);
    }
    return sockfd;
}

void make_address(struct sockaddr_in* address,const char* ip, unsigned short port)
{
    address->sin_family = AF_INET;
    address->sin_port = htons(port);
    if(ip == NULL || strlen(ip) == 0)
        address->sin_addr.s_addr = INADDR_ANY;
    else
        inet_pton(AF_INET,ip,&address->sin_addr.s_addr);
    return;
}
