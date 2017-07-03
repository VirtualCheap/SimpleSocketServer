#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>


std::string* split(std::string text, std::string delimiter, int maxSplits){
    std::string* out = new std::string[maxSplits];
    int index = 0;
    size_t pos = 0;
    std::string token;
    while ((pos = text.find(delimiter)) != std::string::npos) {
        token = text.substr(0, pos);
        out[index] = token;
        text.erase(0, pos + delimiter.length());
        index++;
    }
    return out;
}

void nonblock(int sockfd)
{
    int opts;
    opts = fcntl(sockfd, F_GETFL);
    if(opts < 0)
    {
        fprintf(stderr, "fcntl(F_GETFL) failedn");
    }
    opts = (opts | O_NONBLOCK);
    if(fcntl(sockfd, F_SETFL, opts) < 0)
    {
        fprintf(stderr, "fcntl(F_SETFL) failedn");
    }
}

int main(int argc, char *argv[])
{
     int BUFLEN = 5000;
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[BUFLEN];
     struct sockaddr_in serv_addr, cli_addr;
     int n, i;
     int one = 1;

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) {
        perror("ERROR create socket");
        exit(1);
     }
     setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof one);    //allow reuse of port
     //bind to a local address
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi("5000");
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on bind");
        exit(1);
     }
     //listen marks the socket as passive socket listening to incoming connections,
     //it allows max 5 backlog connections: backlog connections are pending in queue
     //if pending connections are more than 5, later request may be ignored
     listen(sockfd,5);
     //accept incoming connections
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     //nonblock(newsockfd);        //if we want to set the socket as nonblock, we can uncomment this
     if (newsockfd < 0) {
        perror("ERROR on accept");
        exit(1);
     }
     printf("connection accepted\n");
     for (;;) {
         bzero(buffer,BUFLEN);
         n = read(newsockfd,buffer,BUFLEN);
         if (n < 0) {
            perror("ERROR read from socket");
         }

         if (strcmp("", buffer) != 0){
             printf("received: %s %d\n",buffer, strcmp("", buffer));
             std::string bufferStr = buffer;
             std::string* bufferArray = new std::string[3];//split(bufferStr, ",", 3);
             bufferArray[0] = "1.99";
             bufferArray[1] = "1";
             bufferArray[2] = "1";
             float* xyz = new float[3];
             for (int i = 0; i < bufferArray->length(); ++i) {
                xyz[i] = std::stof(bufferArray[i]);
             }
             //std::cout << "X:" << xyz[0] << "Y:" << xyz[1] << "Z:" << xyz[2] << std::endl;
         }
}

//     close(sockfd);
     return 0;
}

