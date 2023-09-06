# SOCKET_PROGRAMMING

This project is about a basic chat application using the TCP protocol in C++. Here's a summary of  chat application's key components and functionality:
1)Server Side:-
i)  The server listens for incoming client connections on a specified port (8080 in your case) using the socket, bind, and listen functions.
ii) When a client connects, the server launches a separate thread (client_handler) to handle that client.
iii)Each client is assigned a unique username, and the server maintains a list of active clients.

2)Client Side:
The client connects to the server using the socket, connect, and write functions. It also sends its chosen username to the server.

3)Communication:
i)Clients can send text messages to the server, which are then broadcast to all connected clients.
ii)The server uses mutexes to protect shared data structures (e.g., the list of active clients) to ensure thread safety.

4)Exit Functionality:
i)If a client sends the message "[CLIENT] exit," it gracefully exits by closing its socket and thread.
ii)The server detects when a client disconnects and removes them from the list of active clients.

5)Header File :
i)You have defined common functions and constants used by both the server and client in a separate header file to promote code reusability.

6)Multithreading:
i)Your code uses the <thread> library to create and manage threads for handling multiple clients concurrently.

7)Error Handling:
i)The code includes error handling to deal with potential issues such as failed socket creation or connection.


Overall, this chat application demonstrates the use of sockets, multithreading, and basic client-server communication. 
