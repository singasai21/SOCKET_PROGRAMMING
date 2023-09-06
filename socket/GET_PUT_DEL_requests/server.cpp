#include <iostream>
#include <cstring>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>
#include <map>
#include <thread>
#include <mutex>
using namespace std;

map<int,string > client_list;
mutex client_mutex;

int server_socket;

map<string, string> database;
mutex database_mutex;

void send_message_to_client(int client_socket, const string& message) {
    write(client_socket, message.c_str(), message.size());
}

void broadcast(int client, const string &message) {
    lock_guard<mutex> lock(client_mutex);
    for (const auto &entry : client_list) {
        if (entry.first != client) {
            send(entry.first, message.c_str(), message.size(), 0);
        }
    }
}

string save(const string &msg,int client_socket) {
    string method = msg.substr(0, 3);
    string rem_msg = msg.substr(3);
    
    string key = rem_msg.substr(0, rem_msg.find('-'));
    string value = rem_msg.substr(rem_msg.find('-') + 1);

    lock_guard<mutex> lock(database_mutex);

    if (method == "PUT") {
        database[key] = value;
        return "value added successfully..";
    } else if (method == "GET") {
        if (database.count(key)) {
            return database[key];
        } else {
            return "Key not present..";
        }
    } else if (method == "DEL") {
        if (database.count(key)) {
            database.erase(key);
            return "successfully deleted";
        } else {
            return "Key not present...";
        }
    }else if(msg=="exit")
    {
            send_message_to_client(client_socket,msg);
            return "";

    } else {
        return "request syntax is invalid..";
    }
}

void checkTheIncomingMessage(int client, const string &name) {
    while (true) {
        char buffer[2048];
        int bytes_read = recv(client, buffer, sizeof(buffer), 0);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            string msg(buffer);
            string res = save(msg,client);
            send(client, res.c_str(), res.size(), 0);
            // broadcast(client, name + ": " + msg);
        } else {
            break;
        }
    }
}

void handleClient(int client) {
    char buffer[2048];
    int bytes_read = recv(client, buffer, sizeof(buffer), 0);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        string name(buffer);
        if (!name.empty()) {
            {
                lock_guard<mutex> lock(client_mutex);
                client_list[client] = name;
            }
            string joiningMsg = name + " joined the chat";
            broadcast(client, joiningMsg);
            checkTheIncomingMessage(client, name);
        }
    }
    close(client);
}

int main() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
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
        thread(handleClient, client_socket).detach();
    }

    close(server_socket);
    return 0;
}
