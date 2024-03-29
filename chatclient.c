// Daniel Rotenberg
// OSU CS 372 Project # 1
// Due 10/28/18

// References:
// Beej's Guide to Network Programming 
// OTP Project from my CS 344 
// https://stackoverflow.com/questions/31103188/my-python-socket-server-can-only-receive-one-message-from-the-client
// https://medium.com/@yashitmaheshwary/simple-chat-server-using-sockets-in-c-f72fc8b5b24e


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

#define BUF_SIZE 500

//variables
int portNum;
char buffer[BUF_SIZE];
char message[BUF_SIZE];
char messageToSend[BUF_SIZE];
char buffer2[BUF_SIZE];
int i = 0;
int charsWritten;
int charsRead;
int serverSocket;
char clientHandle[10];


struct sockaddr_in serv_addr;
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

  memset((char*)&serv_addr, '\0', sizeof(serv_addr)); // Clear out the address struct

  serv_addr.sin_family = AF_INET; // Create a network-capable socket
  serv_addr.sin_port = htons(portNum); // Store the port number   
  serverHostInfo = gethostbyname(argv[1]);  // use argument as target IP address/host
  if (serverHostInfo == NULL) {
  fprintf(stderr, "chatClient: Couldn't connect port # %d\n", portNum);
  exit(1);
  } 

  memcpy((char*)&serv_addr.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

  // Set up the socket
  serverSocket = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
  if (serverSocket < 0) {
      fprintf(stderr, "chatClient: Error opening socket\n");
  }

  // Connect to server
  printf("Connecting to Server\n");
  if (connect(serverSocket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {// Connect socket to address
      fprintf(stderr, "chatClient: Error connecting\n");
      exit(1);
  } else {
      printf("Connected\n");  
  }

  // get client handle and store for future use
  do {
      printf("Please enter a name less than 10 characters: ");
      i = 0;
      // (scanf("%s", clientHandle);
      while((clientHandle[i] = getchar())!='\n') {
          i++;
      }
      clientHandle[i] = '\0';
      // strcpy(stringToSend, clientHandle);
  } while (strlen(clientHandle) < 1 || strlen(clientHandle) > 9);

    // printf("User Handle: %s\n", clientHandle);
    // printf("StringToSend: %s\n", stringToSend);
  printf("Type '\\quit' to exit.\n");

  while(1) {
    printf("%s> ", clientHandle);
    i = 0;
    while((buffer[i] = getchar())!='\n') { 
            i++;
        }
        buffer[i] = '\0';    

    if (strcmp(buffer, "\\quit") == 0) {
        send(serverSocket, "\\quit", 5, 0);
        printf("Quit Command Received, Closing\n");
        break;
    }    

    // printf("Sending %s to the server\n", buffer);
    // Send message to server
    memset(messageToSend, '\0', sizeof(messageToSend));
    strcpy(messageToSend, clientHandle);
    strcat(messageToSend, "> ");
    strcat(messageToSend, buffer);
    // printf("messageToSend: %s\n", messageToSend);

    charsWritten = send(serverSocket, messageToSend, strlen(messageToSend), 0); // Write to the server
    if (charsWritten < 0) {
        fprintf(stderr, "chatClient: Error writing to socket\n");
    }

    if (charsWritten < strlen(messageToSend)) {
        fprintf(stderr, "chatClient: Warning - Not all data written to socket!\n");
    }

    // printf("Getting Return Msg from Server\n");
        // Get return message from server
    charsRead = recv(serverSocket, buffer2, sizeof(buffer2) - 1, 0); // Read data from the socket, leaving \0 at end
    
    // printf("Received Data from Socket\n");
    if (charsRead < 0) {
        fprintf(stderr, "CLIENT: ERROR reading from socket");
    }
    // printf("CLIENT: I received this from the server: \"%s\"\n", buffer2);
    if (strcmp(buffer2, "\\quit") == 0) {
        printf("Quit Command Received From Server, Closing\n");
        break;
    }       
    printf("%s\n", buffer2);

    memset(buffer, 0, sizeof(buffer)); // Clear out the buffer again
    memset(buffer2, 0, sizeof(buffer2)); // Clear out the buffer again
  }

  close(serverSocket);
  return 0;
}