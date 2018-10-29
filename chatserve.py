from socket import *
import sys


BUFFER_SIZE = 501

# https://stackoverflow.com/questions/419163/what-does-if-name-main-do
# https://docs.python.org/release/2.6.5/library/socketserver.html#socketserver-tcpserver-example
# https://docs.python.org/2/library/socket.html

if __name__ == "__main__":
  if len(sys.argv) != 2:  # Make sure correct number of arguments
    print ("Usage: python3 chatServe.py [port #]")
    exit(1)
  
  TCP_PORT = sys.argv[1] # get the port number specificed in the command line
  # print("TCP_PORT: %s" % TCP_PORT) # test if correct port number
  sSocket = socket(AF_INET,SOCK_STREAM) # set up TCP socket
  sSocket.bind(("",int(TCP_PORT))) # bind socket
  sSocket.listen(1) # set up listener for new connections
  while 1:     
    connectSocket, addr = sSocket.accept() # Accept a connection The return value is a pair (conn, address) 
                                             # where conn is a new socket object usable to send and receive data on the connection, 
                                             # and address is the address bound to the socket on the other end of the connection.
    while 1:  
      # print ("Connected on socket %s" % str(connectSocket))    
      # print ("Connected on address %s" % str(addr))
    # while 1:
      try:
        dataFromClient = connectSocket.recv(BUFFER_SIZE)
        if (dataFromClient.decode('utf-8') == "\quit") :
          print("Quit Command Receieved from client, Terminating Connection.")
          print("Waiting for new connection")
          break
        print("%s" % dataFromClient.decode('utf-8'))
        serverName = 'chatServe > '
        response = input("%s" % serverName)
        if (response == "\quit") :
          messageToSend = "\\quit"
          connectSocket.send(messageToSend.encode('utf-8'))          
          print("Quit Command Receieved, Terminating Connection.")
          print("Waiting for new connection")
          break
        
        messageToSend = serverName + response
        # print("Sending this to the client: %s" % response)
        connectSocket.send(messageToSend.encode('utf-8'))
      except KeyboardInterrupt:
        print ("\nKeyboard Interrupt: Exiting Chat")
        sys.exit()
  connectSocket.close()

  #   sentence = connectSocket.recv(BUFFER_SIZE)
  #   print(sentence.decode('utf-8'))
  #   capitalizedSentence = "chatServe > %s" % (sentence.upper())     
  #   connectSocket.send(capitalizedSentence.encode('utf-8'))   
  #   sentence = ''  
  #   connectSocket.close()


