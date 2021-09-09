#include <stdio.h>

struct message {
    char content[1024];
    int seq_num;
};
typedef struct message message;

int server_packet(message packet, int action) {
    /* Function to process a packet on server side
    * seq_num: sequence number of the packet
    * action: 1 for ACK, 2 for NAK
    */
    if (action == 1) {
        return packet.seq_num + 1;
    } else {
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
    int seq_num = 0;
    int action = 0;
    message packet = client_get_packet(messages, n, seq_num);
    while (1) {
        // cout << "[*] client transmitting packet no " << packet.seq_num << endl;
        printf("[*] client transmitting packet no %d\n", packet.seq_num);
        // cout << "Enter the action to perform: \n";
        printf("Enter the action to perform: \n");
        printf("1. ACK\n");
        printf("2. NACK\n");
        // cin >> action;
        scanf("%d", &action);
        seq_num = server_packet(packet, action);

        if (seq_num != packet.seq_num) {
            // update packet
            printf("[*] packet no %d ACKed\n", seq_num);
            packet = client_get_packet(messages, n, seq_num);
        } else {
            // got nack so dont update
            printf("[*] packet no %d NACKed\n", seq_num);
        }

        if (seq_num == n) {
            printf("[*] all packets ACKed\n");
            break;
        }
    }
    
}