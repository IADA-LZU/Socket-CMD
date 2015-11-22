#include <winsock2.h>
#include <stdio.h>
#define PORT 15001
#define BUFFER_SIZE 40960
void Rec()
{
  SOCKET sock2;
  WSAData WSAData;
  SOCKADDR_IN ServerAddr;
  SOCKET msgsock;
  if(WSAStartup(MAKEWORD(2,2),&WSAData)==SOCKET_ERROR)
  {
    //printf("Socket initialize fail!\n");
    exit(1);
  }
  if((sock2=socket(AF_INET,SOCK_STREAM,0))==ERROR)
  {
    //printf("Socket create fail!\n");
    WSACleanup();
    exit(1);
  }
  ServerAddr.sin_family=AF_INET;
  ServerAddr.sin_port=htons(8800);
  ServerAddr.sin_addr.s_addr=INADDR_ANY;
  if(bind(sock2,(LPSOCKADDR)&ServerAddr,sizeof(ServerAddr))==SOCKET_ERROR)
  {
    //printf("Bind fail!\n");
    closesocket(sock2);
    WSACleanup();
    exit(1);
  }
  //printf("Server Socket Port:%d\n",ntohs(ServerAddr.sin_port));
  if(listen(sock2,10)==SOCKET_ERROR)
  {
    //printf("Listen fail!\n");
    closesocket(sock2);
    WSACleanup();
    exit(1);
  }
  char buf[BUFFER_SIZE];
  if((msgsock=accept(sock2,(LPSOCKADDR)0,(int *)0))==INVALID_SOCKET)
    {
      //printf("Accept fail!\n");
      exit(1);
    }
    memset(buf,0,sizeof(buf));
    recv(msgsock,buf,BUFFER_SIZE,0);
    printf("%s\n",buf);
    closesocket(sock2);
    WSACleanup();
}
int main()
{
  char buf[BUFFER_SIZE];
  char buf2[BUFFER_SIZE];
  int inputLen;
  char IP[100];
  WSADATA WSAData;
  SOCKET sock;
  SOCKADDR_IN ClientAddr;
  char Point;
  printf("Would you get the serve IP?(Y or N)");
  Point=getchar();
  //scanf("%c",&Point);
  if(Point=='Y')
  {
      printf("Waiting for the conection...\n");
      Rec();
  }

  printf("input IP:");
  scanf("%s",IP);
  while(1)
  {
    inputLen=0;
    memset(buf,0,sizeof(buf));
    printf("Socket\\Client>");
    getchar();
    while((buf[inputLen]=getchar())!='\n')
    {
      inputLen++;
    }
    if(buf[0]=='e' && buf[1]=='x' && buf[2]=='i' && buf[3]=='t')
    {
      printf("The End.\n");
      exit(1);
    }
    if(WSAStartup(MAKEWORD(2,2),&WSAData)==SOCKET_ERROR)
    {
      printf("Socket initialize fail!\n");
      exit(1);
    }
    if((sock=socket(AF_INET,SOCK_STREAM,0))==SOCKET_ERROR)
    {
      printf("Socket create fail!\n");
      WSACleanup();
      system("pause");
      exit(1);
    }
    ClientAddr.sin_family=AF_INET;
    ClientAddr.sin_port=htons(PORT);
    ClientAddr.sin_addr.s_addr=inet_addr(IP);
    if(connect(sock,(LPSOCKADDR)&ClientAddr,sizeof(ClientAddr))==SOCKET_ERROR)
    {
      printf("Connect fail!\n");
      closesocket(sock);
      WSACleanup();
      system("pause");
      exit(1);
    }
    //fflush(stdin);
    printf("\n");
    send(sock,buf,BUFFER_SIZE,0);
    char ch;
    printf("Would want to get the order information?(Y or N)\n");

    ch=getchar();
    if(ch=='Y')
    {
        memset(buf2,0,sizeof(buf2));
        recv(sock,buf2,BUFFER_SIZE,0);
        printf("%s\n",buf2);
    }

    //closesocket(msgsock);
    closesocket(sock);
    WSACleanup();

  }
  return 0;
}
