// CLIENT2

#include "client.h"

int main() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("socket");
        return 1;
    }

    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if (inet_pton(AF_INET, HOST, &(server_address.sin_addr)) <= 0) {
        perror("inet_pton");
        return 1;
    }

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("connect");
        return 1;
    }

    string username;
    cout << "Enter username: ";
    getline(cin, username);

    if (!username.empty()) {
        write(client_socket, username.c_str(), username.size());
    } else {
        cout << "Invalid username. Username cannot be empty." << endl;
        close(client_socket);
        return 1;
    }

    thread send_thread(handle_send, client_socket);
    thread recv_thread(handle_recv, client_socket);

    send_thread.join();
    recv_thread.join();

    close(client_socket);
    return 0;
}
