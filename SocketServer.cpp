#include<iostream>
#include<winsock.h>

using namespace std;
#define PORT 9909

struct sockaddr_in srv;
fd_set fr, fw, fe;
int nMaxFd;
int nSocket;
int nArrClient[5];


void ProcessNewMessage(int nClientSocket)
{
	cout << "Processing the new message for client socket: " << nClientSocket<<endl;
	char buff[256 + 1] = { 0 };
	int nRet = recv(nClientSocket, buff, 256, 0);
	if (nRet < 0)
	{
		cout << "Something wrong happened closing the socket" << endl;
		closesocket(nClientSocket);
		for (int nIndex = 0; nIndex < 5; nIndex++)
		{
			if (nArrClient[nIndex] == nClientSocket)
			{
				nArrClient[nIndex] = 0;
				break;
			}
		}
	}
	else
	{
		cout << "Message received from client is: " << buff<<endl;
		// send the response to client
		send(nClientSocket, "Processed your request", 23, 0);
		cout << "****************" << endl;
	}
}

void ProcessTheNewRequest()
{
	// New connection request
	if (FD_ISSET(nSocket, &fr))
	{
		int nLen = sizeof(struct sockaddr);
		int nClientSocket = accept(nSocket, NULL, &nLen);
		if (nClientSocket > 0)
		{
			// Put it into client fd_set
			int nIndex;
			for (nIndex = 0; nIndex < 5; nIndex++)
			{
				if (nArrClient[nIndex] == 0) {
					nArrClient[nIndex] = nClientSocket;
					send(nClientSocket, "Got the connection done", 40, 0);
					break;
				}
			}
			if (nIndex == 5)
			{
				cout << "No new space for connection" << endl;
			}
		}
	}
	else
	{
		for (int nIndex = 0; nIndex < 5; nIndex++)
		{
			if (FD_ISSET(nArrClient[nIndex], &fr))
			{
				// Got the new message from the client
				// Just receive the new message
				// Just queue that for new worker to full fill the request
				ProcessNewMessage(nArrClient[nIndex]);
			}
		}
	}
}

int main() {

	int nRet = 0;
	// Initialize the WSA variables
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
	{
		cout << "WSA failed to initialize" << endl;
	}
	else
	{
		cout << "WSA initialized" << endl;
	}
	


	// Initialize the socket
	nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nSocket < 0)
	{
		cout << "The socket is not opened" << endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
	{
		cout << "The socket opened successfully" <<nSocket<< endl;
	}

	// Initialize the environment for socketaddr structure
	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	//srv.sin_addr.s_addr = inet_addr("127.0.0.1");
	srv.sin_addr.s_addr = INADDR_ANY;
	memset(&(srv.sin_zero), 0, 8);


	// About the blocking and non-blocking sockets
	// optval = 0 means blocking and !=0 means non-blocking
	//u_long optval = 1;   for non-blocking socket
	/*
	u_long optval = 0;
	nRet = ioctlsocket(nSocket, FIONBIO, &optval);
	if (nRet != 0)
	{
		cout << "ioctlsocket call failed" << endl;
	}
	else
	{
		cout << "ioctlsocket call passed" << endl;
	}
	*/
	
	// SetSockOpt function called before the bind function
	int noptVal = 0;
	int noptLen = sizeof(noptVal);
	nRet = setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&noptVal, noptLen);
	if (!nRet)
	{
		cout << "The setsockopt is successful" << endl;
	}
	else
	{
		cout << "The setsockopt is failed" << endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	// Bind the socket with local port
	nRet = bind(nSocket,(sockaddr*)&srv, sizeof(sockaddr));
	if (nRet < 0)
	{
		cout << "Fail to bind to local port" << endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
	{
		cout << "Successfully bind to port" << endl;
	}

	// Listen the request from client
	nRet = listen(nSocket, 5);
	if (nRet < 0)
	{
		cout << "Fail to start listen to local port  " << endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
	{
		cout << "Start listening to local port " << endl;
	}

	/*
	int n = 100;
	for (int nIndex = 0; nIndex < 64; nIndex++)
	{
		FD_SET(n, &fr);
		n++;
	}

	cout << fr.fd_count << endl;

	for (int nIndex = 0; nIndex < 99; nIndex++)
	{
		cout << fr.fd_array[nIndex] << " ";
	}
	*/

	nMaxFd = nSocket;
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	

	while (1)
	{
		FD_ZERO(&fr);
		FD_ZERO(&fw);
		FD_ZERO(&fe);

		FD_SET(nSocket, &fr);
		FD_SET(nSocket, &fe);

		for (int nIndex = 0; nIndex < 5; nIndex++)
		{
			if (nArrClient[nIndex] != 0)
			{
				FD_SET(nArrClient[nIndex], &fr);
				FD_SET(nArrClient[nIndex], &fe);
			}
		}

		//cout << "Before select call:" << fr.fd_count << endl;

		// Before select call function it clear all file descriptor
		// Keep waiting for new request and proceed as per requests
		nRet = select(nMaxFd + 1, &fr, &fw, &fe, &tv);
		if (nRet > 0)
		{
			// When someone connects or communicate with someone with dedicated connection
			cout << "Data on port processing now" << endl;
			//break;
			ProcessTheNewRequest();

			// Process the request
			/*
			if (FD_ISSET(nSocket, &fe))
			{
				cout << "There is an exception. Just get away from here" << endl;
			}

			if (FD_ISSET(nSocket, &fw))
			{
				cout << "Ready to write something" << endl;
			}

			if (FD_ISSET(nSocket, &fr))
			{
				cout << "Ready to read. Something new came up to port" << endl;
			}*/
			//break;


		}
		else if (nRet == 0)
		{
			// No connection established or no any request made
			//cout << "Nothing on PORT" << PORT << endl;

		}
		else
		{
			// It fails
			//cout << "I failed" << endl;
			WSACleanup();
			exit(EXIT_FAILURE);
		}

		//cout << "After select call:" << fr.fd_count << endl;
		//Sleep(2000);
	}
	

	return 0;
}
