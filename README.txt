# remote-shell
Building a remote shell using the C programming language and its socket API.

CMPE 150 Final Project
================

   CONTAINS:
      tcp-client.c
      tcp-server.c
      makefile
      README.txt
	  
      tcp-client.c:
         Creates a client through the Socket API allowing the user to send
         server commands. client.c returns the output of the server.
	  
      tcp-server.c:
         Creates a server through the Socket API allowing commands to be
         received by the user. The server emulates a remote shell and returns
         to the user the ouput of commands such as date, ls, and pwd.
	  
      makefile:
         Compiles tcp-client.c and tcp-server.c into executables.
		 
      README.txt:
         Explanation of the project.
		 
   
   COMPILING:
      To compile the project, use the command "make". This will make both the
      client executable and server executable.
      
      To compile individually, type "make server" or "make client"
	  
   
   USAGE:
      Proper usage for the client executable is "./client PORT IPADDRESS". 
      Proper usage for the server executable is "./server PORT". 
      
      An example of these is "./server 1234" and "./client 1234 128.114.104.230".
	  
	  
   RUNNING:
      Being by starting the server with the usage stated. The server will begin
      to listen and wait for commands. Then start the client which will ask for 
      commands to be sent to the server. Once the user inputs an allowed command 
      (date, ls, pwd, shutdown), the client will send the server the commands. 
      The server processes the commands, and the client returns the output of 
      the server.
