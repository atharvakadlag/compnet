#include <bits/stdc++.h>
using namespace std;

#define TIMEOUTTIME 3

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
        {"amet", 4}
    };
    int seq_num = 0;
    int action = 0;
    message packet = client_get_packet(messages, seq_num);
    while (true) {
        cout << "[*] client transmitting packet no " << packet.seq_num << endl;
        cout << "Enter the action to perform: \n";
        cout << "1. ACK\n";
        cout << "2. NACK\n";
        cin >> action;
        seq_num = server_packet(packet, action);

        if (action == 1) {
            // update packet
            cout << "[*] packet no " << seq_num << " ACKed" << endl;
            packet = client_get_packet(messages, seq_num);
        } else {
            // got nack so dont update
            cout << "[*] packet no " << seq_num << " NACKed" << endl;
        }

        if (seq_num == messages.size()) {
            cout << "[*] all packets ACKed" << endl;
            break;
        }
    }
    
}
