#include <bits/stdc++.h>
using namespace std;

#define WINDOWSIZE 3

struct message {
    string content;
    int seq_num;
};

int server_packet(message packet, int action) {
    /* Function to process a packet on server side
    * seq_num: sequence number of the packet
    * action: 1 for ACK, 2 for NAK
    */
    if (action == 1) {
        cout << "[*] server packet no " << packet.seq_num << " ACKed" << endl;
        return packet.seq_num + 1;
    } else {
        cout << "[*] server packet no " << packet.seq_num << " NAKed" << endl;
        return packet.seq_num;
    }
}

message client_get_packet(vector<message> messages, int seq_num) {
    /* Function to process a packet on client side
    * messages: vector of messages
    * seq_num: sequence number of the packet
    */
    // false seq_num not handled
    for (int i = 0; i < messages.size(); i++) {
        if (messages[i].seq_num == seq_num) {
            return messages[i];
        }
    }
    return messages[0];
}

int main() {
    vector<message> messages = {
        {"ipsum", 0},
        {"lorem", 1},
        {"dolor", 2},
        {"sit", 3},
        {"amet", 4},
        {"consectetur", 5},
        {"adipiscing", 6},
    };
    queue<message> window;
    int seq_num = 0;
    int action = 0;
    message packet;
    for (int i = 0; i < WINDOWSIZE; i++) {
        packet = client_get_packet(messages, seq_num);
        cout << "[*] client packet no " << packet.seq_num << " sent" << endl;
        window.push(packet);
        seq_num++;
    }
    while (window.size() > 0) {
        // process the first packet in queue
        packet = window.front();
        window.pop();
        cout << "[*] choose action for packet no " << packet.seq_num << ":\n\t[*] 1 for ACK, 2 for NACK" << endl;
        cin >> action;
        int resp_seq_num = server_packet(packet, action);

        if (packet.seq_num != resp_seq_num) {  // if ack received, remove it from queue
            if (seq_num < messages.size()) {
                packet = client_get_packet(messages, seq_num);
                cout << "[*] client packet no " << packet.seq_num << " sent" << endl;
                window.push(packet);
                seq_num++;
            } else {
                cout << "[*] client sent all packets" << endl;
            }
        } else {  // if nack received requeue the packet
            window.push(packet);
        }
    }
}
