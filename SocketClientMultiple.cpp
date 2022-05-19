//

#include <stdio.h>
#include <iostream>
#include <winsock.h>
#define PORT 9999

using namespace std;

int main()
{
    //Initiate the Socket environment
    WSADATA w;
    int nRet = 0;

    sockaddr_in srv;

    nRet = WSAStartup(MAKEWORD(2, 2), &w);
    if (nRet < 0)
    {
        printf("\nCannot Initialize socket lib");
        return -1;
    }
    //Open a socket - listener
    int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (nSocket < 0)
    {
        //errno is a system global variable which gets updated
        //with the last API call return value/result.
        printf("\nCannot Initialize listener socket:%d", errno);;
        return -1;
    }

    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = inet_addr("192.168.1.8");
    srv.sin_port = htons(PORT);
    memset(&(srv.sin_zero), 0, 8);

    nRet = connect(nSocket, (struct sockaddr*)&srv, sizeof(srv));
    if (nRet < 0)
    {
        printf("\nCannot connect to server:%d", errno);;
        return -1;
    }
    
    cout << endl << "Press Enter to send the message to server";

    getchar();
    //Send the message to server and recieve the response
    int nErr = send(nSocket, "Vikash Kumar Shakya", 255, 0);
    if (nErr < 0)
    {
        //message
    }
    else
    {
        char sBuff[255] = { 0, };
        nErr = recv(nSocket, sBuff, 255, 0);
        if (nErr < 0)
        {
            //Message for failure
        }
        else
        {
            cout << endl << "ACK:" << sBuff;
        }
    }
}
