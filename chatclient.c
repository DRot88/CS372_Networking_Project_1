// Beej's Guide to Network Programming has been used as a reference
// OTP Project from my CS 344 has also been used as a reference

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h> 

#define BUF_SIZE 5000

//variables
int portNum;
char buffer[BUF_SIZE];
char buffer2[BUF_SIZE];
int i;
int charsWritten;
int charsRead;
int serverSocket;
char clientHandle[10];


struct sockaddr_in serverAddress;
struct hostent* serverHostInfo;

// void error(const char *msg) { 
//  // printf("%s\n", msg);
//  fprintf(stderr, "%s", msg); 
//  exit(1); 
// } // Error function used for reporting issues

int main(int argc, char *argv[]) {
    
    if (argc < 3) {
        fprintf(stderr, "Too Few Arguments\n");
        fflush(stdout);
        exit(1);
    } else if (argc > 3) {
        fprintf(stderr, "Too Many Arguments\n");
        fflush(stdout);
        exit(1);
    }

    //store port number
    portNum = atoi(argv[2]); // final argument is the port entered by user
    printf("portNum: %d\n", portNum);

    memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct

    serverAddress.sin_family = AF_INET; // Create a network-capable socket
    serverAddress.sin_port = htons(portNum); // Store the port number   
    serverHostInfo = gethostbyname("localhost");  // use localhost as target IP address/host
    if (serverHostInfo == NULL) {
    fprintf(stderr, "chatClient: Couldn't connect port # %d\n", portNum);
    exit(1);
    } 

    memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

    // Set up the socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
    if (serverSocket < 0) {
        fprintf(stderr, "chatClient: Error opening socket\n");
    }

    // Connect to server
    printf("Connecting to Server\n");
    if (connect(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {// Connect socket to address
        fprintf(stderr, "chatClient: Error connecting\n");
    } else {
        printf("Connected\n");  
    }

    // get client handle and store for future use
    do {
        printf("Please enter a name less than 10 characters: ");
        scanf("%s", clientHandle);  
    } while (strlen(clientHandle) < 1 || strlen(clientHandle) > 9);

    printf("User Handle: %s\n", clientHandle);
while(1) {
    printf("Type something to the server or type '/quit' to exit: ");
    scanf("%s", buffer);
    if (strcmp(buffer, "\\quit") == 0) {
        printf("Quit Command Received, Closing\n");
        break;
    }
    printf("Sending %s to the server\n", buffer);
    // Send message to server
    charsWritten = send(serverSocket, buffer, strlen(buffer), 0); // Write to the server
    if (charsWritten < 0) {
        fprintf(stderr, "chatClient: Error writing to socket\n");
    }

    if (charsWritten < strlen(buffer)) {
        fprintf(stderr, "chatClient: Warning - Not all data written to socket!\n");
    }

    // printf("Getting Return Msg from Server\n");
        // Get return message from server
    charsRead = recv(serverSocket, buffer2, sizeof(buffer2) - 1, 0); // Read data from the socket, leaving \0 at end
    
    // printf("Received Data from Socket\n");
    if (charsRead < 0) {
        fprintf(stderr, "CLIENT: ERROR reading from socket");
    }
    printf("CLIENT: I received this from the server: \"%s\"\n", buffer2);

    memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer again
    memset(buffer2, '\0', sizeof(buffer2)); // Clear out the buffer again
}
    // // ************************************************************** // 

    //     // Send message to server
    // charsWritten = send(serverSocket, keyBuffer, strlen(keyBuffer), 0); // Write to the server
    // if (charsWritten < 0) {
    //     fprintf(stderr, "otp_enc: Error writing to socket\n");
    // }

    // if (charsWritten < strlen(keyBuffer)) {
    //     fprintf(stderr, "otp_enc: Warning - Not all data written to socket!\n");
    // }

    // // printf("Getting Return Msg from Server\n");
    //     // Get return message from server
    // memset(keyBuffer, '\0', sizeof(keyBuffer)); // Clear out the buffer again for reuse
    // charsRead = recv(serverSocket, keyBuffer, sizeof(keyBuffer) - 1, 0); // Read data from the socket, leaving \0 at end
    
    // // printf("Received Data from Socket\n");
    // if (charsRead < 0) {
    //     fprintf(stderr, "CLIENT: ERROR reading from socket");
    // }
    // printf("%s\n", keyBuffer);

    return 0;
}