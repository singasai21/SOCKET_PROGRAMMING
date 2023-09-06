# SOCKET_PROGRAMMING

A combined summary of the chat application and the functionality related to the group GET, PUT, DEL requests:
chat Application with Group Requests (GET, PUT, DEL) over TCP

Overview:
I have developed a TCP-based chat application that allows users to connect, communicate, and perform group-based GET, PUT, and DEL requests. The application is divided into two parts: the server-side and the client-side. It provides real-time chat functionality along with a shared key-value database that can be queried and manipulated by clients using requests.


1)Server-side:
*The server listens on a specified port (8080) for incoming client connections using sockets.
*It maintains a list of connected clients with their respective usernames, ensuring thread safety with a mutex.
*The server handles client connections and launches a separate thread for each connected client.
*Messages sent by clients are broadcasted to all other clients in the chat room.
*The server provides a shared key-value database for clients to perform GET, PUT, and DEL operations.
*Error handling is implemented for socket operations and client connections.


2)Client-side:
*Clients connect to the server using sockets and provide a unique username.
*Clients can send text messages to the chat room, which are broadcasted to all connected clients.
*Clients can perform GET, PUT, and DEL requests on the shared key-value database.
*Error handling is implemented for socket operations and connection to the server.

3)Group Requests (GET, PUT, DEL):
*Clients can send requests in the format: <METHOD>-<KEY>-<VALUE> to perform operations on the shared database.
*Supported methods include PUT (for adding/updating values), GET (for querying values), and DEL (for deleting values).
*The server validates and processes these requests, providing appropriate responses to clients.
*Clients can interact with the database to store and retrieve key-value pairs.
*Invalid requests are handled gracefully with appropriate error messages.

4)Additional Notes:
*The code implements multithreading for handling multiple clients concurrently.
*Mutexes are used to ensure thread safety when accessing shared data structures (client list, database).
*Clients can gracefully exit the chat by sending an "exit" message.
*Comprehensive error handling is implemented for robustness.
