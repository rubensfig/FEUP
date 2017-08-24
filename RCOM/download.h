#ifndef	HEADERFILE
#define HEADERFILE

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <string.h>
#include <errno.h> 
#include <netdb.h> 
#include <netinet/in.h> 

#include "download.h"

#define SERVER_PORT  21	
#define STRING_SIZE 255
#define USER 0
#define PASS 1
#define PASV 2
#define RETR 3
#define LIST 4

char* SERVER_ADDR;

int reader(int socketfd, char* buffer);
char* command(char* buf, int command, int* size);
int clientTCP(char* host, char* url, char* user, char* filename);
int getip(char* url);
char* divide_url(char* url, char* user, char* filename);
int new_ip(char* novo);

#endif
