#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int s_sock;
    int c_socket;
    char buf[100] = "hello client\n";
    char buf2[100];
    s_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server, other;
    memset(&server, 0, sizeof(server));
    memset(&other, 0, sizeof(other));
    server.sin_family = AF_INET;
    server.sin_port = htons(9009);
    server.sin_addr.s_addr = INADDR_ANY;
    socklen_t add;
    bind(s_sock, (struct sockaddr*)&server, sizeof(server));
    listen(s_sock, 5);
    add = sizeof(other);
    c_socket = accept(s_sock, (struct sockaddr*)&other, &add);

    send(c_socket, buf, sizeof(buf), 0);

    int choice = 1;
    while (1) {
        // get choice from user
        printf("1. send message\n2. recv message\n3. close connection\n");
        scanf("%d", &choice);
        if (choice == 1) {
            // get string from user
            printf("enter your message: ");
            // flush the buffer
            fflush(stdin);
            fgets(buf, sizeof(buf), stdin);
            send(c_socket, buf, sizeof(buf), 0);
        } else if (choice == 2) {
            recv(c_socket, buf2, sizeof(buf2), 0);
            printf("%s", buf2);
        } else if (choice == 3) {
            break;
        } else {
            printf("invalid choice {%d}\n", choice);
        }
    }

    close(s_sock);
    return 0;
}
