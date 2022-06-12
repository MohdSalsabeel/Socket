#include<iostream >
#include<winsock.h>

using namespace std;

#define PORT 9090

struct hostent* remoteHost;
struct in_addr addr;
struct sockaddr_in srv;
int nClientSocket;
char** pAlias;

int main()
{

	
	
	WSADATA ws;
	int nStatus = WSAStartup(MAKEWORD(2, 2), &ws);
	if (nStatus < 0)
	{
		cout << endl << "WSAStartup failed";
		exit(EXIT_FAILURE);
	}
	//cout << nStatus << endl;

	int nListener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


	char szHostName[32] = { 0, };
	
	remoteHost = gethostbyname(szHostName);
	//cout << remoteHost->h_name << endl;
	if (szHostName[0] != '\0')
	{

		for (pAlias = remoteHost->h_aliases; *pAlias != 0; pAlias)
		{
			cout << *pAlias << endl;
		}
		//cout << "Address type: " << endl;
		//cout << remoteHost->h_addrtype << endl;

		switch (remoteHost->h_addrtype)
		{
		case AF_INET:
			//cout << "AF_INET" << endl;
			srv.sin_family = AF_INET;
			break;
		case AF_NETBIOS:
			cout << "AF_NETBIOS" << endl;
			break;
		default:
			break;
		}

		//cout << "Address length: " << remoteHost->h_length << endl;
		int i = 0;
		if (remoteHost->h_addrtype == AF_INET)
		{
			while (remoteHost->h_addr_list[i] != 0)
			{
				addr.s_addr = *(u_long*)remoteHost->h_addr_list[i++];
				cout << "IP address: " << inet_ntoa(addr);
			}
		}

		else if (remoteHost->h_addrtype == AF_NETBIOS)
		{
			cout << "NETBIOS address is returned" << endl;
		}


		srv.sin_port = htons(PORT);
		srv.sin_addr.s_addr = inet_addr(inet_ntoa(addr));
		memset(&(srv.sin_zero), 0, 8);

		nStatus = bind(nListener, (struct sockaddr*)&srv, sizeof(srv));
		cout << "bind " << nStatus << endl;
		if (nStatus < 0)
		{
			cout << endl << "bind failed";
			exit(EXIT_FAILURE);
		}

		nStatus = listen(nListener, 5);
		if (nStatus < 0)
		{
			cout << endl << "listen failed";
			exit(EXIT_FAILURE);
		}

		//accept generates a new socket id
		//this new socket id is used for communication with client
		int nLen = sizeof(srv);
		//int nClientSocket = accept(nListener, NULL, &nLen);
		//SOCKET nClientSocket;
		nClientSocket = accept(nListener, NULL, &nLen);
		cout << "accept " << nClientSocket << endl;

		
	}

	if (nClientSocket == INVALID_SOCKET)
	{
		cout << endl << "Client socket not connected";
		exit(EXIT_FAILURE);
	}
	else
	{
		char sBuff[1024] = { 0, };
		char sBuffc[200];
		//loop, communicate with client
		while (1)
		{
			nStatus = recv(nClientSocket, sBuff, 1024, 0);
			if (nStatus > 0)
			{
				cout << endl << sBuff;
			}
			cout << endl << "Please enter the message:";
			gets_s(sBuffc, 200);
			nStatus = send(nClientSocket, sBuffc, strlen(sBuffc), 0);
			if (nStatus > 0)
			{
				cout << "Message sent" << endl;
			}


		}
		//char hl[12] = "hello";
		//send(nClientSocket, hl, strlen(hl), 0);
	}
	

	


	//Stream socket : Connection oriented
	//Datagram socket: connectionless socket
	/*
	int nListener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nListener < 0)
	{
		cout << endl << "socket API failed to open listener";
		exit(EXIT_FAILURE);
	}
	cout << "Listener " << nListener << endl;

	//closesocket(nListener);

	//htons : host to network short
	//Needed for Network byte order (Big Endian)
	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	//srv.sin_addr.s_addr = INADDR_ANY;
	srv.sin_addr.s_addr = inet_addr("192.168.0.105");
	memset(&srv.sin_zero, 0, sizeof(srv.sin_zero));

	//cout << "srv.sin_family: " << srv.sin_family << endl;
	//cout << "srv.sin port is: " << srv.sin_port << endl;
	//cout << "srv.sin_addr.s_addr: " << srv.sin_addr.s_addr << endl;

	nStatus = bind(nListener, (struct sockaddr*)&srv, sizeof(srv));
	cout << "bind " << nStatus << endl;
	if (nStatus < 0)
	{
		cout << endl << "bind failed";
		exit(EXIT_FAILURE);
	}

	nStatus = listen(nListener, 5);
	if (nStatus < 0)
	{
		cout << endl << "listen failed";
		exit(EXIT_FAILURE);
	}

	//accept generates a new socket id
	//this new socket id is used for communication with client
	int nLen = sizeof(srv);
	//int nClientSocket = accept(nListener, NULL, &nLen);
	SOCKET nClientSocket;
	nClientSocket = accept(nListener, NULL, &nLen);
	cout <<"accept " << nClientSocket << endl;
	if (nClientSocket == INVALID_SOCKET)
	{
		cout << endl << "Client socket not connected";
		exit(EXIT_FAILURE);
	}
	else
	{
		char sBuff[1024] = { 0, };
		char sBuffc[200];
		//loop, communicate with client
		while (1)
		{
			nStatus = recv(nClientSocket, sBuff, 1024, 0);
			if (nStatus > 0)
			{
				cout << endl << sBuff;
			}
			cout << endl << "Please enter the message:";
			gets_s(sBuffc, 200);
			nStatus = send(nClientSocket, sBuffc, strlen(sBuffc), 0);
			if (nStatus > 0)
			{
				cout << "Message sent" << endl;
			}
			
			
		}
		//char hl[12] = "hello";
		//send(nClientSocket, hl, strlen(hl), 0);
	}
	*/

	return 0;
}
