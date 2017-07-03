#include "udp_server.h"

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSIZE 2048

int
main(int argc, char **argv)
{
    struct sockaddr_in myaddr;	/* our address */
    struct sockaddr_in remaddr;	/* remote address */
    socklen_t addrlen = sizeof(remaddr);		/* length of addresses */
    int recvlen;			/* # bytes received */
    int fd;				/* our socket */
    char *buf;	/* receive buffer */

    /* create a UDP socket */

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("cannot create socket\n");
        return 0;
    }

    /* bind the socket to any valid IP address and a specific port */

    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(50000);

    if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
        perror("bind failed");
        return 0;
    }
    bool conneted = false;
    while (!conneted) {
        buf[0] = 0;
        recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
        if (recvlen > 0) {
            buf[recvlen] = 0;
            std::cout << "received message: \"%s\" (%d bytes)" << buf << std::endl;
            if(strcmp(buf,"connect")==0){
                std::cout << "connected"  << std::endl;
                std::cout << sendto(fd, buf, sizeof(buf), 0, (struct sockaddr *)&remaddr, addrlen) << std::endl;
                if (sendto(fd, buf, sizeof(buf), 0, (struct sockaddr *)&remaddr, addrlen) < 0){
                    std::cout << "connected"  << std::endl;
                    conneted = true;
                }
            }
        }else{
            std::cout <<  "uh oh - something went wrong!" << std::endl;
        }
    }
    // now loop, receiving data and printing what we received
    /*for (;;) {
        buf[0] = 0;
        std::cout << "waiting on port 50000" << std::endl;
        recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
        if (recvlen > 0) {
            buf[recvlen] = 0;
            std::cout << "received message: \"%s\" (%d bytes)" << buf << recvlen << std::endl;
        }
        else
            std::cout <<  "uh oh - something went wrong!" << std::endl;
        std::cout << "sending response \n" << buf  << std::endl;
        if (sendto(fd, buf, sizeof(buf), 0, (struct sockaddr *)&remaddr, addrlen) < 0)
            std::cout << "sendto"  << std::endl;
    }*/


}
