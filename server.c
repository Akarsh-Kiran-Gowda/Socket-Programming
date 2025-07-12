// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT "3490"  // Port to listen on
#define BACKLOG 10   // Max pending connections
#define BUFSIZE 1024

int main() {
    struct addrinfo hints, *res;
    int sockfd, new_fd;
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    char buf[BUFSIZE];
    int bytes_received;

    // 1. Prepare hints
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_flags = AI_PASSIVE;      // For bind()

    // 2. Get address info
    getaddrinfo(NULL, PORT, &hints, &res);

    // 3. Create socket
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    // 4. Bind it
    bind(sockfd, res->ai_addr, res->ai_addrlen);

    // 5. Listen
    listen(sockfd, BACKLOG);
    printf("Server: waiting for connections on port %s...\n", PORT);

    // 6. Accept connection
    addr_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
    printf("Server: got connection!\n");

    // 7. Receive data
    bytes_received = recv(new_fd, buf, BUFSIZE-1, 0);
    buf[bytes_received] = '\0';
    printf("Server: received '%s'\n", buf);

    // 8. Echo back
    send(new_fd, buf, bytes_received, 0);

    // 9. Cleanup
    close(new_fd);
    close(sockfd);
    freeaddrinfo(res);
    return 0;
}
