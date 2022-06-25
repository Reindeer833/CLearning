#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define MAXPENDING   5
#define RCVBUFSIZE   64
#define TERMINATE    "q!"

void DieWithError(char* errorMessage);
void HandleTCPClient(int clntSocket);

int main()
{

    int servSock;
    int clntSock;

    struct sockaddr_in echoServAddr;
    struct sockaddr_in echoClntAddr;
    unsigned echoServPort;
    unsigned int clntLen;

    echoServPort = 1122;

/* Create socket for incoming connections   */
    if( (servSock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP) ) <0 )
    {
        DieWithError("socket() failed");
    }
    memset(&echoServAddr,0,sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;    /* Internet address family  */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);  /* Any incoming interface  */
    echoServAddr.sin_port = echoServPort;  /* Local port  */

/*  Bind to the local address    */
    if(bind(servSock,(struct sockaddr*)&echoServAddr,sizeof(echoServAddr)) < 0)
    {
        DieWithError("bind() failed");
    }

/* Mark the socket so it will listen for incomming connections  */
    if(listen(servSock,MAXPENDING) < 0)
    {
        DieWithError("listen() failed");
    }
    for(;;)
    {

    printf("\n server");
    sleep((2));
/* Set the size of the in-out parameter   */
        clntLen = sizeof(echoClntAddr);

/* Wait for a client  to connect  */
        if( (clntSock = accept(servSock,(struct sockaddr*)&echoClntAddr,&clntLen) ) < 0 )
        {
            DieWithError("accept() failed");
        }
/* clntSock is connected to client    */

        printf("Handling client: %s\n",inet_ntoa(echoClntAddr.sin_addr));
        HandleTCPClient(clntSock);
    }



    return 0;
}

void DieWithError(char* errorMessage)
{
    perror(errorMessage);
    exit(1);
}

void HandleTCPClient(int clntSocket)
{
  char echoBuffer[RCVBUFSIZE];
  int recvMsgSize;

  /* Receive message from client   */
  if((recvMsgSize = recv(clntSocket,echoBuffer,RCVBUFSIZE,0)) < 0)
  {
            DieWithError("recv() failed");
  }

  while(recvMsgSize > 0)  // zero indicates end of transmission
  {

        if(0 == strncmp(TERMINATE,echoBuffer,strlen(TERMINATE)) )
        {
                printf("\nReceived termination command from client\n");
                break;
        }

        if(send(clntSocket,echoBuffer,recvMsgSize,0) != recvMsgSize)
        {
            DieWithError("send() failed");
        }


       if((recvMsgSize = recv(clntSocket,echoBuffer,RCVBUFSIZE,0)) < 0)
       {
                DieWithError("recv() failed");
       }


  }
  close(clntSocket)  ;
}
