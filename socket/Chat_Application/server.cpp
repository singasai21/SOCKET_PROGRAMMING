#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <thread>
#include <mutex>
#include <arpa/inet.h>
#include <unistd.h>

std::mutex client_list_mutex;
std::vector<std::pair<std::string, int>> active_clients; // List of all currently connected users

// Function to send message to a single client
void send_message_to_client(int client_socket, const std::string& message) {
    write(client_socket, message.c_str(), message.size());
}

// Function to send any new message to all the clients that
// are currently connected to this server
void send_messages_to_all(const std::string& message ,const std :: string& username) {
    std::lock_guard<std::mutex> lock(client_list_mutex);
    
    for (const auto& user : active_clients) {
        if(user.first!=username){
        send_message_to_client(user.second, message);
        }
    }
}

// Function to listen for upcoming messages from a client
void listen_for_messages(int client_socket, const std::string& username) {
    while (true) {
        char buffer[2048];
        int bytes_read = read(client_socket, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            break; // Connection closed or error occurred
        }
        
        buffer[bytes_read] = '\0';
        std::string message(buffer);
        std::cout << message << std::endl;
         if(message=="[CLIENT] exit")
         {
            std::string mes="exit";
            send_message_to_client(client_socket,mes);
            close(client_socket);
            exit;
         }
        if (!message.empty()) {
            std::string final_msg = username + "~" + message;
            send_messages_to_all(final_msg,username);
        }
    }
}

// Function to handle client
void client_handler(int client_socket) {
    char username_buffer[2048];
    int bytes_read = read(client_socket, username_buffer, sizeof(username_buffer));
    if (bytes_read <= 0) {
        close(client_socket);
        return;
    }

    username_buffer[bytes_read] = '\0';
    std::string username(username_buffer);
//    std :: cout << username << std :: endl;
    if (!username.empty()) {
        {
            std::lock_guard<std::mutex> lock(client_list_mutex);
            active_clients.push_back(std::make_pair(username, client_socket));
        }
        std::string prompt_message = "SERVER~" + username + " added to the chat";
        send_messages_to_all(prompt_message,username);
        listen_for_messages(client_socket, username);
    }

    close(client_socket);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        return 1;
    }

    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("bind");
        return 1;
    }

    if (listen(server_socket, 5) == -1) {
        perror("listen");
        return 1;
    }

    while (true) {
        sockaddr_in client_address{};
        socklen_t client_address_size = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_size);
        if (client_socket == -1) {
            perror("accept");
            continue;
        }

        std::thread client_thread(client_handler, client_socket);
        client_thread.detach(); // Detach thread to allow it to run independently
    }

    close(server_socket);
    return 0;
}