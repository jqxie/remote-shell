#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int sockfd, newsockfd, portno;
char buffer[256], b1[256];
socklen_t clilen;
struct sockaddr_in serv_addr, cli_addr;
int i, j, n, mode=0, acpt=1;
FILE *file;

void error(char *msg){
  perror(msg);
  exit(1); //exit systems
}

void input(int argc, char *argv[]){
  //allocating specific memory size to the serv_addr
  bzero((char *) &serv_addr, sizeof(serv_addr)); //memory at *

  portno = atoi(argv[1]); //converts string to int in 2nd argument 

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  if (argc < 2){ // 2 arguments to run command: stdio + port#
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1); //exit if no port
  }

  sockfd = socket(AF_INET, SOCK_STREAM, 0); //creating new socket
  if (sockfd < 0){ //cannot receive anything because no socket
    error("ERROR opening socket");
  }
}

//connects
void cliconn(int sockfd){
  if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &acpt, sizeof(acpt))
  ==-1){
    perror("setsockopt");
    error("ERROR in reusing address");
  }

  //binding client socket to server socket
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))
  <0){
    error("ERROR in binding");
  }

  //only needs to accept once
  //creating a connection queue
  if (listen(sockfd,5)==-1){
    error("ERROR in listening\n");
  }
  clilen = sizeof(cli_addr);

  //accepting from client socket
  newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
  if (newsockfd < 0){
    error("ERROR in accepting");
  }
}

//closes old sockets in client
int cliclose(int n, int sockfd, int newsockfd, char *buffer){
  //reading more than once || read and translate
  if (n < 0){
    error("ERROR reading from socket");
  }
  if (n == 0){
    close(newsockfd);
    close(sockfd);
    return 0;
  }

  //Stops transmission from client, closes socket
  if(strcmp(buffer, "shutdown\n")==0){
    shutdown(newsockfd, 1); //further sends are disallowed
    close(sockfd);
    return 0;
  }
  return 1;
}
void execute(int sockfd, int newsockfd, char *buffer, char *b1){
  buffer[strlen(buffer)-1] = 0; //'\n' part of string
  strcat(buffer, "> file"); //concatenates buffer & the file named file
  b1[0] = '\0'; //clear output
  i = 0; //reseting index file

  //duplicates calling process of the system output, returns to client
  if(fork() == 0){
    dup2(newsockfd, 0); //copy file desc to 0
    dup2(newsockfd, 1); //copy file desc to 1
    dup2(newsockfd, 2); //copy file desc to 2
    close(newsockfd); //closes socket
    execlp("sh", "sh", "-c", buffer, (char *)0); //sh
  } else{
    wait(&mode);
  }

  //opens file containing server's temporary output & returns to client
  file = fopen("file", "r");
  while((j = getc(file))!=EOF){
    b1[i++] = j;
    b1[i] = '\0';
  }
  fclose(file);

  //output feedback to client
  if (write(newsockfd, b1, strlen(b1))<0){
     error("ERROR writing sockets");
  }
}

int main(int argc, char *argv[]){
  for(;;){ //infinite loop
    input(argc, argv);
    cliconn(sockfd);
    for(;;){
      bzero(buffer, 256); //allocation
      i = recv(newsockfd, buffer, 255, 0);
      if(cliclose(i, sockfd, newsockfd, buffer)==0){
        break;
      }
      execute(sockfd, newsockfd, buffer, b1);
    } while(i>0); //do loop while i >0
  }
  return 0;
}
