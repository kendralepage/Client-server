/*
 * Author: Kendra LePage
 * File: client.c
 * Date: 10.14.2019
 * Description: Socket Client using TCP as transport
 * Note:
 *  The inputs are: ip address, port number and a string to pass to the server.
 *  Ex. client 127.0.0.1 5555 "Welcome to CSCI 345!"
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
 #include <arpa/inet.h>
#endif

int main(int argc, char *argv[])
{

#ifdef WIN32
	WSADATA wsaData;
	SOCKET clientsock = INVALID_SOCKET;
#else
    int clientsock;
#endif

    int rc = 0;
    int port = 0;
    char inputbuffer[512] = {'\0'};
    /* int addresslength = 0; */
    /* char outputbuffer[512] = {'\0'}; */

    struct sockaddr_in send_to;
    /* struct sockaddr recv_from; */

    if (argc < 4) {
        fprintf(stdout,"Usage: %s <ip address> <port> <string>\n", argv[0]);
        return -1;
    }

#ifdef WIN32
    /* If receiving an error upon startup (!=0) return string and error */
	rc = WSAStartup(MAKEWORD(2,2), &wsaData);
	if ( rc != 0 ) {
       fprintf(stdout,"WSAStartup failed with error: %d\n",rc);
	   return rc;
	}
#endif

    /* If string 2 is empty set the port to 4444, if found convert arg 2 to int from sting */
    if ( strcmp(argv[2],"") ) {
        port = 4444;
    } else {
        port = atoi(argv[2]);
    }

    /* Set clientsock to accept ipv4 and communicate using tcp
    return error if socket value is less than zero, if successful print string*/
    clientsock = socket(AF_INET, SOCK_STREAM, 0);
    if ( clientsock < 0 ) {
        perror("Error opening socket");
        return rc;
    } else {
        fprintf(stdout, "Client socket created...\n");
    }

    /* Send a message to storage include size of the message */
    memset(&send_to,'\0',sizeof(send_to));

    /* Send message to address from argument one & convert ipv4 address to binary,
    send message to host
     */
    send_to.sin_addr.s_addr = inet_addr(argv[1]);
    send_to.sin_family = AF_INET;
    send_to.sin_port = htons(port);

    /* if error code is not 0 return error, specific error code for windows
    return connected if no error code (rc=0) */
    rc = connect(clientsock, (struct sockaddr *)&send_to, sizeof(send_to));
    if ( rc < 0 ) {
        perror("Error connecting");
#ifdef WIN32
        fprintf(stdout,"Error from winsock: %d\n",WSAGetLastError());
#endif
        return rc;
    } else {
        fprintf(stdout, "Client socket connected...\n");
    }

    /* If there is no input in arg3 copy the input buffer and an empty space, if not empty, 
    send argument 3 */
    if ( strcmp(argv[3], "") == 0 ) {
        strcpy(inputbuffer, "");
    } else {
        strcpy(inputbuffer, argv[3]);
    }

    /* Set rc to error code of sending message to the client socket with input buffer, size of input buffer, the group of variables that make up
    sock address, and send it to the client socket if error is not 0 print error. If successful print write to*/
    rc = sendto(clientsock, inputbuffer, sizeof(inputbuffer), 0, (struct sockaddr *)&send_to, sizeof(send_to) );
    if ( rc < 0 ) {
        perror("Error writing socket");
#ifdef WIN32
        fprintf(stdout,"Error from Winsock: %d\n",WSAGetLastError());
#endif
        return rc;
    } else {
        fprintf(stdout, "----> Client socket write to server: %s\n", inputbuffer);
    }

     /*
    addresslength = sizeof(recv_from);
    rc = recvfrom(clientsock, outputbuffer, sizeof(outputbuffer), 0, &recv_from, &addresslength);
    if ( rc < 0 ) {
        perror("Error reading socket");
#ifdef WIN32
        fprintf(stdout,"Error from Winsock: %d\n",WSAGetLastError());
#endif
        return rc;
    } else {
        fprintf(stdout, "----> Client socket read: %s\n", outputbuffer);
    }
    */

#ifdef WIN32
    /* Run close command if windows */
    rc = closesocket(clientsock);
#else
    /* Run close command if other operating system */
    rc = close(clientsock);
#endif

    if ( rc < 0 ) {
        perror("Error closing socket");
        return rc;
    } else {
        fprintf(stdout, "Client socket closed...\n");
    }

#ifdef WIN32
	WSACleanup();
#endif

    return rc;
}



