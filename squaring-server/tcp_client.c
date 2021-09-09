#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    int c_sock;
    char buf[100];
    c_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));  // optional
    client.sin_family = AF_INET;
    client.sin_port = htons(9009);
    client.sin_addr.s_addr = INADDR_ANY;
    if (connect(c_sock, (struct sockaddr*)&client, sizeof(client)) == -1) {
        printf("error in connection");
        return 0;
    }

    recv(c_sock, buf, sizeof(buf), 0);
    printf("message from server:%s", buf);

    int choice = 1;
    while (1) {
        // get choice from user
        printf("1. send message\n2. recv message\n3. close connection\n");
        scanf("%d", &choice);
        if (choice == 1) {
            // get string from user
            printf("enter your message: ");
            fflush(stdin);
            fgets(buf, sizeof(buf), stdin);
            // send string to server
            send(c_sock, buf, strlen(buf), 0);
        } else if (choice == 2) {
            // wait and recv
            recv(c_sock, buf, sizeof(buf), 0);
            printf("message from server:%s", buf);
        } else if (choice == 3) {
            break;
        } else {
            printf("invalid choice\n {%d}", choice);
        }
    }

    close(c_sock);

    return 0;
}
