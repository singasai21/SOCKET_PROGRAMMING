
#ifndef client
#define client

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>
#include <map>
#include <thread>
#include <mutex>
#include <bits/stdc++.h>

using namespace std;

const char *HOST = "127.0.0.1";
const int PORT = 8080;

void handle_send(int client_socket) {
    while (true) {
        string data;
        cout << "Enter message to send: ";
        getline(cin, data);

        if (!data.empty()) {
            write(client_socket, data.c_str(), data.size());
        }
    }
}

void handle_recv(int client_socket) {
    while (true) {
        char buffer[2048];
        int bytes_read = read(client_socket, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            cout << "Connection closed." << endl;
            break;
        }

        buffer[bytes_read] = '\0';
          string s(buffer);
        
        if(s=="exit")
        {
            close(client_socket);
            exit(1);
        }
        cout << buffer << endl;
    }
}


#endif 
