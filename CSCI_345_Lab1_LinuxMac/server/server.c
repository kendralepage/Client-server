/*
 * Author: Kendra_LePages
  * File: server.c
 * Date: 10.14.2019
 * Description: Socket Server using TCP as the transport and entry point for a client connection
 * Note:
 *  The inputs are: port number.
 *  Ex. server 5555
 *
 * Edits
 *
 * 10/01/2019 SDN - Code creation
 *
 */
#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
 #include <windows.h>
 #include <winsock2.h>
 #include <ws2tcpip.h>
#else
 #include <string.h>
 #include <unistd.h>
 #include <netdb.h>
#endif

int main(int argc, char *argv[])
{

#ifdef WIN32
	WSADATA wsaData;
	SOCKET listensocket = INVALID_SOCKET;
	SOCKET acceptsocket = INVALID_SOCKET;
#else
    int listensocket;
    int acceptsocket;
#endif

    int rc = 0;
    int port = 0;
    int clientaddrlen = 0;
    int fromaddresslength = 0;
    char buffer[255] = {'\0'};

    struct sockaddr_in serveraddress, clientaddress;
    struct sockaddr recv_from;

    if (argc < 2) {
        fprintf(stdout,"Usage: server <port>\n");
        return -1;
    }

#ifdef WIN32
    /* On startup of windows machine check for error code */
	rc = WSAStartup(MAKEWORD(2,2), &wsaData);
	if ( rc != 0 ) {
       fprintf(stdout,"WSAStartup failed with error: %d\n",rc);
	   return rc;
	}
#endif

    /* if argument one is empty set it to 4444, if it's not empty convert it to an integer */
    if ( strcmp(argv[1],"") ) {
        port = 4444;
    } else {
        port = atoi(argv[2]);
    }

    /* set listen socket to address family, socket stream, 0. if no input in first two return error */
    listensocket = socket(AF_INET, SOCK_STREAM, 0);
    if ( listensocket < 0 ) {
        perror("Error opening socket");
        return rc;
    } else {
        fprintf(stdout, "Server socket has been created...\n");
    }

    /* fill a block of memory with server address, nul, and the size of server address */
    memset(&serveraddress,'\0',sizeof(serveraddress));

    /* Send message to address from argument one & convert ipv4 address to binary,
    send message to host */
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_addr.s_addr = INADDR_ANY;
    serveraddress.sin_port = htons(port);

    /* bind a name to a socket, using listen socket, combining sockaddr and server address
    and the size of server address. return error code if failed */
    rc = bind(listensocket, (struct sockaddr *) &serveraddress, sizeof(serveraddress));
    if ( rc < 0 ) {
        perror("Error binding");
        return rc;
    } else {
        fprintf(stdout, "Server socket can bind a port...\n");
    }

    /* listen for connection on socket, if failed return error */
    rc = listen(listensocket, 5);
    if ( rc < 0 ) {
        perror("Error Listening");
        return rc;
    } else {
        fprintf(stdout, "Server socket is listening...\n");
    }

    clientaddrlen = sizeof(clientaddress);

    while(1) {
        /* run until break in statement is issued. accept incoming message on socket. return error if failed */
        acceptsocket = accept(listensocket, (struct sockaddr *) &clientaddress, &clientaddrlen);
        if ( acceptsocket < 0 ) {
           perror("Error creating client socket");
           return rc;
       } else {
           fprintf(stdout, "----> Client socket is connected...\n");
       }

       /* receive message from the accepting socket including the buffer, size of buffer, and the
       sender information  */
       fromaddresslength = sizeof(recv_from);
       rc = recvfrom(acceptsocket, buffer, sizeof(buffer),0, (struct sockaddr *)&recv_from, &fromaddresslength);
       if ( rc < 0 ) {
           perror("Error reading client socket");
           return rc;
       } else {
           fprintf(stdout, "----> Received data from client: %s\n", buffer);
       }

#ifdef WIN32
    /* closing socket on windows */
    rc = closesocket(acceptsocket);
#else
    /* closing socket on mac/linux */
    rc = close(acceptsocket);
#endif

       if ( rc < 0 ) {
           perror("Error closing client socket");
           return rc;
       } else {
           fprintf(stdout, "----> Client socket is closed...\n");
       }
    } // End While Loop

#ifdef WIN32
    /* remove any leftover data from open socket. */
	WSACleanup();
#endif
    return rc;
}



