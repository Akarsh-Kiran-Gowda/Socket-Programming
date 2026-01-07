// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT "3490"  // Port to connect to
#define BUFSIZE 1024

int main(int argc, char *argv[]) {
    struct addrinfo hints, *res;
    int sockfd;
    char buf[BUFSIZE];
    int bytes_received;
    char *message = "Hello from client!";

    // Check if hostname is provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
        fprintf(stderr, "Example: %s localhost\n", argv[0]);
        exit(1);
    }

    // 1. Prepare hints
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP

    // 2. Get address info
    if (getaddrinfo(argv[1], PORT, &hints, &res) != 0) {
        fprintf(stderr, "getaddrinfo failed\n");
        exit(1);
    }

    // 3. Create socket
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        fprintf(stderr, "socket creation failed\n");
        exit(1);
    }

    // 4. Connect to server
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        close(sockfd);
        fprintf(stderr, "connection failed\n");
        exit(1);
    }
    printf("Client: connected to server\n");

    // 5. Send data
    if (send(sockfd, message, strlen(message), 0) == -1) {
        fprintf(stderr, "send failed\n");
        exit(1);
    }
    printf("Client: sent '%s'\n", message);

    // 6. Receive echo
    bytes_received = recv(sockfd, buf, BUFSIZE-1, 0);
    if (bytes_received == -1) {
        fprintf(stderr, "recv failed\n");
        exit(1);
    }
    buf[bytes_received] = '\0';
    printf("Client:  received echo '%s'\n", buf);

    // 7. Cleanup
    close(sockfd);
    freeaddrinfo(res);
    return 0;
}
