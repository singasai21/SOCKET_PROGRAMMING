
#ifndef common
#define common
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include<bits/stdc++.h>

const char *HOST = "127.0.0.1";
const int PORT = 8080;

void handle_send(int client_socket) {
    while (true) {
        std::string data;
        std::cout << "Enter message to send: ";
        std::getline(std::cin, data);

        if (!data.empty()) {
            data = "[CLIENT] " + data;
            write(client_socket, data.c_str(), data.size());
        }
    }
}

void handle_recv(int client_socket) {
    while (true) {
        char buffer[2048];
        int bytes_read = read(client_socket, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            std::cout << "Connection closed." << std::endl;
            break;
        }

        buffer[bytes_read] = '\0';
        std:: string s(buffer);
        if(s=="exit")
        {
            close(client_socket);
            exit(0);
        }
        std::cout << buffer << std::endl;
    }
}
#endif 
