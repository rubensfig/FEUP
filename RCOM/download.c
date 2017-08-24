/*      (C)2000 FEUP  */

#include "download.h"

int reader(int socketfd, char* buffer){
    int i = 0;
    
    while(1){	
	i++;
	if(*(buffer+i) != '\n') break;	
    }
        
    return i;
}

char* command(char* buf, int command, int* size)
{    
    char* new_str = malloc(STRING_SIZE);
    char* str;
    
    strcpy(new_str, "");
    
    switch(command){
	case(0):
	    str = "user ";
	    strcat(new_str, str);
	    strcat(new_str, buf);
	    *size = strlen(new_str);
	    new_str[*size] = '\n';
	    return new_str;
	case(1):
	    str = "pass ";
	    strcat(new_str, str);
	    strcat(new_str, buf);
	    *size = strlen(new_str);
	    new_str[*size] = '\n';
	    return new_str;
	case(2):
	    str = "pasv";
	    strcat(new_str, str);
	    *size = strlen(new_str);
	    new_str[*size] = '\n';
	    return new_str;
	case(3):
	    str = "retr ";
	    strcat(new_str, str);
	    strcat(new_str, buf);
	    *size = strlen(new_str);
	    new_str[*size] = '\n';
	    return new_str;
	case(4):
	    str = "list";
	    strcat(new_str, str);
	    *size = strlen(new_str);
	    new_str[*size] = '\n';
	    return new_str;
    }   
    
    return NULL;
}

int new_ip(char* novo)
{
    char * pch;
    int p[6];
    int i = 0;

    //printf ("Splitting string \"%s\" into tokens:\n",novo);
    pch = strtok (novo,"(.,)");

    while (pch != NULL)
    {			
	p[i] = atoi(pch);
	i++;
	pch = strtok (NULL, " (.,)");	
    }

  // printf("LAST TWO DIGITS:%d %d\nPORT NUMBER: %d\n", p[5], p[6], 256*p[5] + p[6]);

    return p[5]*256+p[6];

}

int clientTCP(char* host, char* url, char* user, char* filename){

	int	sockfd;
	int	clsockfd;
	struct	sockaddr_in server_addr;
	struct 	sockaddr_in client_addr;
	char*	bufe = malloc(STRING_SIZE); 
	char* 	buffer = malloc(STRING_SIZE);
	char* 	aux = malloc(STRING_SIZE);
	char* 	aux2 = malloc(STRING_SIZE);
	char*	CLIENT_ADDR = malloc(STRING_SIZE);
	int	bytes, i = 1;
	int	size_of_str;
	int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);	
	char*	ReadBuffer[4096];
	
	fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	/*server address handling*/
	bzero((char*)&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);	/*32 bit Internet address network byte ordered*/
	server_addr.sin_port = htons(SERVER_PORT);		/*server TCP port must be network byte ordered */
    	
	/*open an TCP socket*/
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
    		perror("socket()");
        	exit(0);
    	}
	
	/*connect to the server*/
    	if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        	perror("connect()");
		exit(0);
	}	
	
	/*send a string to the server*/
	while(1){
	    if(user[i] == ':')   break;
	    aux2[i-1] = user[i];
	    i++;	    
	}

	strncpy(aux, user + i + 1, strlen(user) - 13);	
	//printf("USER : %s\nPASS : %s\n", aux2, aux);

	read(sockfd, buffer, STRING_SIZE);
	write(STDOUT_FILENO, buffer, strlen(buffer));
	memset(buffer, 0, strlen(buffer));

	bufe = command(aux2, USER, &size_of_str);
	write(STDOUT_FILENO, bufe, size_of_str+1);
	bytes = write(sockfd, bufe, size_of_str+1);

	read(sockfd, buffer, STRING_SIZE);	
	write(STDOUT_FILENO, buffer, strlen(buffer));
	memset(buffer, 0, strlen(buffer));

	bufe = command(aux, PASS, &size_of_str);
	write(STDOUT_FILENO, bufe, size_of_str+1);
	bytes = write(sockfd, bufe, size_of_str+1);

	read(sockfd, buffer, STRING_SIZE);	
	write(STDOUT_FILENO, buffer, strlen(buffer));
	memset(buffer, 0, strlen(buffer));

	bufe = command(aux, PASV, &size_of_str);
	write(STDOUT_FILENO, bufe, size_of_str+1);
	bytes = write(sockfd, bufe, size_of_str+1);

	read(sockfd, buffer, STRING_SIZE);	
	write(STDOUT_FILENO, buffer, strlen(buffer));
	
	bzero((char*)&client_addr,sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);	/*32 bit Internet address network byte ordered*/
	client_addr.sin_port = htons(new_ip(buffer));		/*server TCP port must be network byte ordered */
	
	memset(buffer, 0, strlen(buffer));
	
	/*open an TCP socket*/
	if ((clsockfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
    		perror("socket()");
        	exit(0);
    	}
	
	/*connect to the server*/
    	if(connect(clsockfd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0){
        	perror("connect()");
		exit(0);
	}	   
	
	bufe = command(url, RETR, &size_of_str);
	write(STDOUT_FILENO, bufe, size_of_str+1);
	bytes = write(sockfd, bufe, size_of_str+1);

	read(sockfd, buffer, STRING_SIZE);	
	write(STDOUT_FILENO, buffer, strlen(buffer));
	
	
	if(strncmp(buffer, "150", 3) != 0){
	    fprintf(stderr, "Error Ocurred, File Probably not Found.\n");

	    exit(-1);
	}
	memset(buffer, 0, strlen(buffer));
	
	while((bytes = read(clsockfd, ReadBuffer, 4096)) > 0)
	{
	    write(fd, ReadBuffer, bytes);	    
	}	

	read(sockfd, buffer, STRING_SIZE);	
	write(STDOUT_FILENO, buffer, strlen(buffer));
	memset(buffer, 0, strlen(buffer));
	
	bytes = write(sockfd, "quit", 5);	
	
	close(sockfd);
	close(clsockfd);
	return 0;
}

int getip(char* url)
{   
    	struct hostent* h;

        if ((h=gethostbyname(url)) == NULL) {  
            herror("gethostbyname");
            exit(1);
        }

        printf("Host name  : %s\n", h->h_name);
        printf("IP Address : %s\n", inet_ntoa(*((struct in_addr *)h->h_addr)));
	
	strcpy(SERVER_ADDR, inet_ntoa(*((struct in_addr *)h->h_addr)));
	
        return 0;
}

char* divide_url(char* url, char* user, char* filename)
{
    char* host = malloc(STRING_SIZE);
    char* str = malloc(STRING_SIZE);
    int i = 0, j = 0;
    int state = 0;
    int pos = 0;
    
    while(1){
	
	//printf("%c\n", url[i]);
	if(url[i] == '[') state = 1;
	if(state == 1)
	{
	    user[j] = url[i];
	    j++;	
	    if(url[i] == ']'){
		state = 2;
		i++;
		j = 0;
		continue;
	    }
	}   
	if(state == 2)
	{
	    
	   if(url[i] == '/') break;
	   host[j] = url[i];
	   j++;
	}
	i++;
    }
    
    strncpy(str, url + i + 1, strlen(url) - i);
    strncpy(url, "", STRING_SIZE);
    strncpy(url, str, strlen(str));
    
    i = 0;
    
    while(url[i] != '\0')
    {
	if(url[i] == '/') pos = i;
	i++;
    }
    
    if(i = 0) filename[0] = '\n';
    else strncpy(filename, url + pos + 1, strlen(url));
   // write(STDOUT_FILENO, filename, strlen(url));
    
    
   // printf("USER : %s\nHOST : %s\nURL : %s\n", user, host, url);
    
   // free(str);
    
    return host;   
}	

int main(int argc, char *argv[])
{
    
    char* url;
    url = malloc(STRING_SIZE); 
    
    strcpy(url, argv[1]);    
    
    char* user = malloc(STRING_SIZE);
    char* filename = malloc(STRING_SIZE);
    char* host = divide_url(url, user, filename);    
    
    
    SERVER_ADDR = malloc(STRING_SIZE);
         
    if(host == NULL)
    {
	fprintf(stderr, "usage : ftp://[<user>:<password>@]<host>/<url-path>\n");
	printf("\n");
	return -1;
    }
    
    getip(host);
    
  //  printf("HOST : %s\nUSER : %s\nURL : %s\n", host ,user, url);
        
    clientTCP(host, url, user, filename);
    
    return 0;
    
}


