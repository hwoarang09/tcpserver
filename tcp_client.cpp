#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <memory.h>
#include "network_utils.h"




#pragma comment(lib, "ws2_32.lib")

/*
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
*/
#include <memory.h>

#include <errno.h>


#define DEST_PORT            40000
#define SERVER_IP_ADDRESS   "127.0.0.1"

#define SRC_PORT	     40001
#define LOCAL_IP_ADDRESS "127.0.0.1"

typedef struct _test_struct {

    unsigned int a;
    unsigned int b;
} test_struct_t;


typedef struct result_struct_ {

    unsigned int c;

} result_struct_t;

test_struct_t client_data;
result_struct_t result;

void
setup_tcp_communication() {
    
    WSADATA wsaData;
    int result2 = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result2 != 0) {
        printf("WSAStartup failed: %d\n", result2);
        exit(1);
    }

    int sockfd = 0,
        sent_recv_bytes = 0;

    socklen_t addr_len = 0;

    addr_len = sizeof(struct sockaddr);


    struct sockaddr_in dest;

    dest.sin_family = AF_INET;

    dest.sin_port = htons(DEST_PORT);
    printf("1\n");
    struct hostent* host = (struct hostent*)gethostbyname(SERVER_IP_ADDRESS);
    if (host == NULL) {
        printf("gethostbyname failed\n");
        WSACleanup();
        exit(0);
    }
    printf("2\n");
    dest.sin_addr = *((struct in_addr*)host->h_addr);
    printf("3\n");
    /*Step 3 : Create a TCP socket*/
    /*Create a socket finally. socket() is a system call, which asks for three paramemeters*/
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#if 0
    /*to specify the client IP Address and Port no*/
    struct sockaddr_in localaddr;
    localaddr.sin_family = AF_INET;
    localaddr.sin_addr.s_addr = inet_addr(LOCAL_IP_ADDRESS);
    localaddr.sin_port = htons(SRC_PORT);  // Any local port will do
    bind(sockfd, (struct sockaddr*)&localaddr, sizeof(localaddr));
#endif
    printf("Connecting to Server\n");
    int rc = connect(sockfd, (struct sockaddr*)&dest, sizeof(struct sockaddr));

    if (rc == 0) {
        printf("connected\n");
    }
    else {
        printf("connection failed, error no %d\n", errno);
        exit(0);
    }

    /*Step 4 : get the data to be sent to server*/
    /*Our client is now ready to send data to server. sendto() sends data to Server*/

    printf("hihi\n");
PROMPT_USER:

    /*prompt the user to enter data*/
    printf("Enter a : ?\n");
    scanf("%u", &client_data.a);
    printf("Enter b : ?\n");
    scanf("%u", &client_data.b);

    if (client_data.a == 0 && client_data.b == 0) {
        //close(sockfd);
        closesocket(sockfd);
        exit(0);
    }

    char buffer[sizeof(test_struct_t)];
    memcpy(buffer, &client_data, sizeof(test_struct_t));

    /*step 5 : send the data to server*/
    sent_recv_bytes = sendto(sockfd,
        buffer,
        sizeof(test_struct_t),
        0,
        (struct sockaddr*)&dest,
        sizeof(struct sockaddr));

    printf("No of bytes sent = %d\n", sent_recv_bytes);

    /*Step 6 : Client also want to reply from server after sending data*/

    /*recvfrom is a blocking system call, meaning the client program will not run past this point
     * untill the data arrives on the socket from server*/
    printf("Waiting for response:\n");
    sent_recv_bytes = recvfrom(sockfd, (char*)&result, sizeof(result_struct_t), 0,
        (struct sockaddr*)&dest, &addr_len);

    printf("No of bytes recvd = %d\n", sent_recv_bytes);

    printf("Result recvd = %u\n", result.c);
    /*Step 7: Client would want to send the data again to the server, go into infinite loop*/
    goto PROMPT_USER;
}

/*
int
main(int argc, char** argv) {
    printf("hi");
    setup_tcp_communication();
    printf("application quits\n");
    return 0;
}
*/