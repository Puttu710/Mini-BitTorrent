#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include<bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <arpa/inet.h>
#define _BSD_SOURCE
using namespace std;
#define bcopy(s1, s2, n) memmove((s2), (s1), (n))
#define bzero(s, n) memset((s), 0, (n))


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];//to store the msg
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     // create a socket
     // socket(int domain, int type, int protocol)
     sockfd =  socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
// clear address structure
     bzero((char *) &serv_addr, sizeof(serv_addr));

     portno = atoi(argv[1]);

     /* setup the host_addr structure for use in bind call */
     serv_addr.sin_family = AF_INET;  

     // automatically be filled with current host's IP address
     serv_addr.sin_addr.s_addr = INADDR_ANY;  

     // convert short integer value for port must be converted into network byte order
     serv_addr.sin_port = htons(portno);

     
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");


     int status=listen(sockfd,5);
     
        clilen = sizeof(cli_addr);

     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");

// This send() function sends the 13 bytes of the string to the new socket
    int count= send(newsockfd, "Hello, world!\n", 13, 0);
    //count stores no of bytes transmitted -1 if error

     bzero(buffer,256);
      FILE *fp=fopen("tester.txt","wb");
    int file_size;
    recv(sockfd, &file_size,sizeof(file_size),0);
    
    while((n=recv(newsockfd,buffer,256,0))>0&& file_size>0)
    {
        fwrite(buffer,sizeof(char),n,fp);
        memset(buffer,'\0',256);
        file_size=file_size-n;
    }
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);
      
     close(newsockfd);
     //}
     close(sockfd);
     return 0; 
}

