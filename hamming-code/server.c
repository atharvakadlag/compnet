#include <math.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define HOST "localhost"
#define PORT 8080
#define SA struct sockaddr

void create_connection(int* sockfd_p, int* connfd_p) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(1);
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    if (bind(sockfd, (SA*)&server, sizeof(server)) == -1) {
        printf("Socket binding failed...\n");
        exit(1);
    }

    if (listen(sockfd, 3) == -1) {
        printf("Listen failed...\n");
        exit(1);
    }

    struct sockaddr_in client;
    int client_len = sizeof(client);

    int connfd = accept(sockfd, (SA*)&client, &client_len);
    if (connfd == -1) {
        printf("Connection accepting failed...\n");
        exit(1);
    }

    *sockfd_p = sockfd;
    *connfd_p = connfd;
}

int get_r(int n) {
    int r = 0;
    while (n + r + 1 > pow(2, r)) {
        r++;
    }
    printf("r = %d\n", r);
    return r;
}

void parse_input(int connfd, int* op_p, int* n_p, char** bitstream_p) {
    char buffer[1024] = {0};
    if (read(connfd, buffer, sizeof(buffer)) < 0) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    // parse the buffer to get number of bytes and bitstream
    char* token = strtok(buffer, " ");
    int op = atoi(token);

    token = strtok(NULL, " ");
    int n = atoi(token);

    char* bitstream = malloc(n * sizeof(char));
    token = strtok(NULL, " ");
    strncpy(bitstream, token, n);
    bitstream[n] = '\0';

    *op_p = op;
    *n_p = n;
    *bitstream_p = bitstream;
    printf("op = %d, n = %d, bitstream = %s\n", *op_p, *n_p, bitstream);
}

int* get_code(int n, char* bitstream, size_t* size) {
    printf("n = %d bitstream = %s\n", n, bitstream);

    int r = get_r(n);
    int length = n + r;
    int* code = malloc(length * sizeof(int));

    // generate the code (non hamming)
    int j = 0, k = 0;
    for (int i = 0; i < length; i++) {
        if (i == pow(2, k) - 1) {
            code[i] = 0;
            k++;
        } else {
            code[i] = bitstream[j] - '0';
            j++;
        }
    }

    // set parity bits
    for (int i = 0; i < r; i++) {
        int pos = pow(2, i);

        int count = 0;
        int k = pos - 1;  // store the starting bit of block

        while (k < length) {
            for (int j = k; j < k + pos && j < length; j++) {  // traverse the block
                count += code[j];
            }
            k += 2 * pos;  // go to the next block
        }

        code[pos - 1] = count % 2;  // set the parity bit
    }
    *size = length;
    return code;
}

int get_error(int length, char* bitstream) {
    int r = 0;
    while (pow(2, r) < length) {
        r++;
    }
    int* parity = malloc(r * sizeof(int));

    // convert bitstream to code
    int* code = malloc(length * sizeof(int));
    for (int i = 0; i < length; i++) {
        code[i] = bitstream[i] - '0';
    }
    printf("code = %s, r = %d\n", bitstream, r);
    // set parity bits
    for (int i = 0; i < r; i++) {
        int pos = pow(2, i);

        int count = 0;
        int k = pos - 1;  // store the starting bit of block

        while (k < length) {
            for (int j = k; j < k + pos && j < length; j++) {  // traverse the block
                count += code[j];
            }

            k += 2 * pos;  // go to the next block
        }

        parity[i] = count % 2;  // set the parity bit
    }
    // print the parity bits
    for (int i = 0; i < r; i++) {
        printf("%d", parity[i]);
    }
    printf("\n");
    // convert binary array into decimal
    int error = 0;
    for (int i = 0; i < r; i++) {
        error += parity[i] * pow(2, i);
    }
    return error;
}

int main() {
    int sockfd, connfd;
    create_connection(&sockfd, &connfd);

    char* buffer[1024] = {0};
    if (read(connfd, buffer, sizeof(buffer)) < 0) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    printf("%s\n", buffer);

    while (1) {
        // get raw bytes from client and encode them
        printf("Waiting for data...\n");
        size_t size;
        // int* code = get_code(connfd, &size);
        int op, n; 
        char* bitstream;
        parse_input(connfd, &op, &n, &bitstream);
        if (op == 1) { // generate code
            int* code = get_code(n, bitstream, &size);

            // get string from code
            char* str = malloc(size * sizeof(char));
            for (int i = 0; i < size; i++) {
                str[i] = code[i] + '0';
            }
            str[size] = '\0';

            // send the encoded data to client
            printf("Sending encoded data...%s\n", str);
            if (write(connfd, str, size * sizeof(int)) < 0) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        } else if (op == 2) { // check code
            int error = get_error(n, bitstream);
            // convert integer to string
            char* str = malloc(sizeof(char) * 10);
            sprintf(str, "%d%c", error, '\0');
            // itoa(error, str, 10);

            printf("Sending error...%s\n", str);
            if (write(connfd, str, strlen(str)) < 0) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
    }

    return 0;
}
