Developer: Daniel Rotenberg
Project # 1: Chat Server + Client Program

How To Use:

This was testing on 'flip1' (OSU Server), but can be changed depending on the server you're using. I have also testing it locally using 'localhost'.

First, connect the server program by typing "python3 chatserve.py [port #]" into the terminal. Port # can be anything, but it's best to choose a non standard number (I tested with ports greater than 2000).

Next, in a separate terminal window, type "make" to run the makefile program which will compile the chatclient.c file.

To run the newly compiled file, type "./chatClient [flip1] [port #]" where the port # is the same number used for the server.

To execute the beginning of the chat, follow the commands on the client terminal until your message has been sent. At any time, either the client or the server can end the connection by typing '\quit'. 