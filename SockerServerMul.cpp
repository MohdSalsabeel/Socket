//Server code
#include<iostream>
#include <winsock.h>
#include<fstream>

#define PORT 9999

using namespace std;
//TCP socket server handling multiple clients


int nListener = 0;
struct sockaddr_in srv;
int nMaxFd = 0;
fd_set fr;

int nClient[5] = { 0, };

void AcceptConnection()
{
	int nClientSocket = accept(nListener, NULL, NULL);
	if (nClientSocket < 0)
	{
		cout << endl << "Not able to accept the new client connection";
		return;
	}

	int nIndex;
	for (nIndex = 0; nIndex < 5; nIndex++)
	{
		if (nClient[nIndex] == 0)
		{
			nClient[nIndex] = nClientSocket;
			if (nMaxFd < nClientSocket)
			{
				nMaxFd = nClientSocket + 1;
			}
			break;
		}
	}
	if (nIndex == 5)
	{
		cout << endl << "Server is busy..";
	}
}

void RecvResponseMsg(int nSocket)
{
	//First recieve the message from client
	char sMsg[255] = { 0, };
	int nErr = recv(nSocket, sMsg, 255, 0);
	if (nErr < 0)
	{
		cout << endl << "Failed to get the message..";
		//Close the connection to this client socket
		for (int nIndex = 0; nIndex < 5; nIndex++)
		{
			if (nClient[nIndex] == nSocket)
			{
				nClient[nIndex] = 0;
				closesocket(nSocket);
				break;
			}
		}
	}
	else
	{
		//Print the message 
		cout << endl << "The message from client:" << sMsg;
		//send an acknowledgement to client
		strcpy_s(sMsg, "Successfully Recieved the message.");
		nErr = send(nSocket, sMsg, strlen(sMsg), 0);
		if (nErr < 0)
		{
			cout << endl << "Failed to send the message..";
			//Close the connection to this client socket
			for (int nIndex = 0; nIndex < 5; nIndex++)
			{
				if (nClient[nIndex] == nSocket)
				{
					nClient[nIndex] = 0;
					closesocket(nSocket);
					break;
				}
			}

		}
		else
		{
			cout << endl << "Sent the message..";
		}
	}

	
}

int main()
{
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
	{
		cout << endl << "The WSAStartup failed..";
		return EXIT_FAILURE;
	}

	nListener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nListener < 0)
	{
		cout << endl << "Not able to open the socket..";
		return EXIT_FAILURE;
	}

	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = INADDR_ANY;
	memset(&srv.sin_zero, 0, 8);

	int err = bind(nListener, (struct sockaddr*)&srv,
		sizeof(sockaddr));
	if (err < 0)
	{
		cout << endl << "The bind failed..";
		return EXIT_FAILURE;
	}

	err = listen(nListener, 5);
	if (err < 0)
	{
		cout << endl << "The listen failed..";
		return EXIT_FAILURE;
	}

	nMaxFd = nListener + 1;
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	while (1)
	{
		FD_ZERO(&fr);
		FD_SET(nListener, &fr);
		int nIndex;
		for (nIndex = 0; nIndex < 5; nIndex++)
		{
			if (nClient[nIndex] != 0)
			{
				FD_SET(nClient[nIndex], &fr);
			}
		}

		err = select(nMaxFd, &fr, NULL, NULL, &tv);
		if (err < 0)
		{
			cout << endl << "Select failed..";
			return EXIT_FAILURE;
		}
		else if (err == 0)
		{
			//cout << endl << "No client available to connect/message";
		}
		else
		{
			
			//Rest of the processing to connect/recv the req
			if (FD_ISSET(nListener, &fr))
			{
				cout << endl << "Someone connected..";
				//Accept the new connection
				AcceptConnection();
			}
			else
			{
				for (int nIndex = 0; nIndex < 5; nIndex++)
				{
					if (FD_ISSET(nClient[nIndex], &fr))
					{
						//Need to recieve the message from client
						RecvResponseMsg(nClient[nIndex]);
					}
				}
			}
		}
	}
	return 0;
}
