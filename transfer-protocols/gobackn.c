#include <stdio.h>
#include "queue.h"

#define WINDOWSIZE 3

int server_packet(message packet, int action) {
    /* Function to process a packet on server side
    * seq_num: sequence number of the packet
    * action: 1 for ACK, 2 for NAK
    */
    if (action == 1) {
        // cout << "[*] server packet no " << packet.seq_num << " ACKed" << endl;
        printf("[*] server packet no %d ACKed\n", packet.seq_num);
        return packet.seq_num + 1;
    } else {
        // cout << "[*] server packet no " << packet.seq_num << " NAKed" << endl;
        printf("[*] server packet no %d NAKed\n", packet.seq_num);
        return packet.seq_num;
    }
}

message client_get_packet(message* messages, int num, int seq_num) {
    /* Function to process a packet on client side
    * messages: vector of messages
    * seq_num: sequence number of the packet
    */
    // false seq_num not handled
    for (int i = 0; i < num; i++) {
        if (messages[i].seq_num == seq_num) {
            return messages[i];
        }
    }
    return messages[0];
}

int main() {
    int n = 5;
    message messages[5] = {
        {"ipsum", 0},
        {"lorem", 1},
        {"dolor", 2},
        {"sit", 3},
        {"amet", 4}
    };

    struct Queue* window = createQueue(5);
    int seq_num = 0;
    int action = 0;
    message packet;
    for (int i = 0; i < WINDOWSIZE; i++) {
        packet = client_get_packet(messages, n, seq_num);
        printf("[*] client packet no %d sent\n", packet.seq_num);
        enqueue(window, packet);
        seq_num++;
    }
    while (!isEmpty(window)) {
        // process the first packet in queue
        packet = front(window);
        // cout << "[*] choose action for packet no " << packet.seq_num << ":\n\t[*] 1 for ACK, 2 for NACK" << endl;
        printf("[*] choose action for packet no %d:\n\t[*] 1 for ACK, 2 for NACK\n", packet.seq_num);
        scanf("%d", &action);
        int resp_seq_num = server_packet(packet, action);

        if (packet.seq_num != resp_seq_num) {  // if ack received, remove it from queue
            dequeue(window);
            if (seq_num < n) {
                packet = client_get_packet(messages, n, seq_num);
                printf("[*] client packet no %d sent\n", packet.seq_num);
                enqueue(window, packet);
                seq_num++;
            } else {
                printf("[*] client packet no %d sent\n", packet.seq_num);
            }
        } else {  // if nack received or ack lost move the seq_num back by WINDOWSIZE
            printf("[*] client packet no %d NAKed\n", packet.seq_num);
            printf("[*] clearing window\n");
            while (!isEmpty(window)) {
                dequeue(window);
            }
            printf("[*] go back %d packets\n", WINDOWSIZE);
            seq_num = resp_seq_num; // go back to last packet which is same as going back WINDOWSIZE packets
            for (int i = 0; i < WINDOWSIZE && seq_num < n; i++) {
                packet = client_get_packet(messages, n, seq_num);
                printf("[*] client packet no %d sent\n", packet.seq_num);
                enqueue(window, packet);
                seq_num++;
            }
        }
    }
}
