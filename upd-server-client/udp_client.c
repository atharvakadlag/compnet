#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>

int main()
{
    int c_sock;
    char msg[256] = "I am client";
    c_sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in client, other;
    client.sin_family = AF_INET;
    client.sin_port = htons(9001);
    client.sin_addr.s_addr = INADDR_ANY;
    socklen_t ad;
    ad = sizeof(other);
    if (sendto(c_sock, msg, sizeof(msg), 0, (struct sockaddr *)&client, sizeof(client))) {
        printf("Client sent:%s \n", msg);
    } else {
        printf("Unable to send the message");
    }
    close(c_sock);
    return 0;
}