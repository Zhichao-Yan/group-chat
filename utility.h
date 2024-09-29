#ifndef U_H
#define U_H

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>



int create_socket();
void make_address(struct sockaddr_in* address,const char* ip, unsigned short port);
#endif