#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    int c_sock;
    char buf[100];
    char buf2[100] = "pong";

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
    printf("Message from server: %s\n", buf);

    send(c_sock, buf2, sizeof(buf2), 0);

    close(c_sock);

    return 0;
}
