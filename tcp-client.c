#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

int sockfd, portno, n;
struct sockaddr_in serv_addr;
struct hostent *server;
const struct sockaddr *cast = (struct sockaddr *)&serv_addr; //constant
char buffer[256];

void error(char *msg){
  perror(msg);
  exit(0);
}

void initialize(char *argv[]){
  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd<0){
    error("ERROR opening socket");
  }

  server = gethostbyname(argv[1]);
  if (server == NULL){
    fprintf(stderr, "ERROR, no such host\n");
    exit(0);
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family=AF_INET;

  bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,
  server->h_length);

  serv_addr.sin_port = htons(portno);
  //connect to server
  if (connect(sockfd,cast,sizeof(serv_addr)) < 0){
    error("ERROR connecting");
  }
}

void cmd(char *buffer, int sockfd){
  while(1){ //infinite loop
    //preparing message
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));

    if (n < 0)
      error("ERROR writing socket");

    if(buffer[0]==10) //skip if command is enter
      continue;
    if (strcmp(buffer, "shutdown\n")==0){ //shutdown -> close connection
      shutdown(sockfd, 2);
      exit(1); //exit system from loop
    }

    bzero(buffer, 256);
    if(recv(sockfd, buffer, 255, 0) < 0){
      error("ERROR reading socket"); //error msg
    }
    printf("%s\n",buffer);
  }
}

int main(int argc, char *argv[]){
  if(argc < 3){ //0, 1, 2 from client
    fprintf(stderr, "Usage %s hostname port\n", argv[0]);
    exit(0);
  }
  initialize(argv);
  cmd(buffer, sockfd);
  return 0;
}
