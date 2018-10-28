from socket import *

TCP_PORT = 12001
BUFFER_SIZE = 1024

sSocket = socket(AF_INET,SOCK_STREAM)
sSocket.bind(("",TCP_PORT))
sSocket.listen(1)
print ("The server is ready to receive")
while 1:     
  connectSocket, addr = sSocket.accept()

  sentence = connectSocket.recv(BUFFER_SIZE)
  capitalizedSentence = "chatserve > %s" % (sentence.upper())     
  connectSocket.send(capitalizedSentence.encode('utf-8'))     
  connectSocket.close()


  # modded = "Modded: %s" % (sentence.upper())