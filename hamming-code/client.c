#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define HOST "localhost"
#define PORT 8080
#define SA struct sockaddr

int create_connection() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Error opening the socket");
        exit(1);
    }

    struct sockaddr_in server;
    server.sin_addr = *(struct in_addr*)gethostbyname(HOST)->h_addr;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if (connect(sockfd, (SA*)&server, sizeof(server)) == -1) {
        printf("Error connecting to the server\n");
    }
    return sockfd;
}

int main() {
    int sockfd = create_connection();
    char* message = "Hello World\n";
    if (send(sockfd, message, strlen(message), 0) == -1) {
        printf("Error sending message\n");
        exit(1);
    }
    printf("All standard sizes supported. 7, 8, 15, 16, 31, 32...\n");
    printf("Operations supported: generate (gen) / check (chk)\n");
    while (1) {
        char buffer[1024] = {0};
        // get buffer form stdin
        printf("Enter message<op size msg>: ");
        fgets(buffer, 1024, stdin);
        // send buffer to server
        // if buffer = 'exit', break

        if (send(sockfd, buffer, strlen(buffer), 0) == -1) {
            printf("Error sending message\n");
            exit(1);
        }

        if (strcmp(buffer, "exit\n") == 0) {
            close(sockfd);
            break;
        }

        // get buffer from server
        char reply[1024] = {0};
        if (recv(sockfd, reply, 1024, 0) == -1) {
            perror("Error receiving message\n");
            exit(1);
        } else {
            printf("Server replied: %s\n", reply);
        }
    }
    return 0;
}