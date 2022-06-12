#include <iostream>
#include <winsock.h>
#include <string.h>

using namespace std;

#define PORT 9090

struct sockaddr_in srv;
struct hostent* remoteHost;
struct in_addr addr;
int nStatuss;

int main()
{
	WSADATA ws;
	
	int nStatus = WSAStartup(MAKEWORD(2, 2), &ws);
	if (nStatus < 0)
	{
		cout << endl << "Failed to call WSAStartup";
		exit(EXIT_FAILURE);
	}

	char** pAlias;
	char szHostName[32] = { 0, };

	int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nSocket < 0)
	{
		cout << endl << "Not able to call socket successfully";
		exit(EXIT_FAILURE);
	}

	int iRet = gethostname(szHostName, 32);
	if (iRet != 0)
	{
		cout << "Not able to find the host name " << endl;
		return 1;
	}
	else
	{
		cout << "The host name is: " << szHostName << endl;
	}

	remoteHost = gethostbyname(szHostName);
	//cout << remoteHost->h_name << endl;
	if (szHostName[0] != '\0')
	{

		//for (pAlias = remoteHost->h_aliases; *pAlias != 0; pAlias)
		//{
		//	cout << *pAlias << endl;
		//}
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
				//cout << "IP address: " << inet_ntoa(addr);
			}
		}

		else if (remoteHost->h_addrtype == AF_NETBIOS)
		{
			cout << "NETBIOS address is returned" << endl;
		}

		srv.sin_port = htons(PORT);
		srv.sin_addr.s_addr = inet_addr(inet_ntoa(addr));
		memset(&(srv.sin_zero), 0, 8);

		nStatuss = connect(nSocket, (struct sockaddr*)&srv, sizeof(srv));
		cout << "connect: " << nStatus << endl;

		if (nStatuss < 0)
		{
			cout << endl << "connect failed";
			exit(EXIT_FAILURE);
		}
		

	}
	
		char sBuffr[1024] = { 0, };
		while (1)
		{
			char sBuff[200];
			char sBuffc[1024] = { 0, };
			cout << endl << "Please enter the message:";
			gets_s(sBuff, 200);

			nStatuss = send(nSocket, sBuff, 200, 0);
			if (nStatus < 0)
			{
				cout << endl << "The send failed. Exitting..";
				exit(EXIT_FAILURE);
			}
			else if (nStatuss > 0)
			{
				cout << endl << "Message sent!!" << endl;
			}

			nStatuss = recv(nSocket, sBuffc, 200, 0);
			if (nStatuss > 0) {
				cout << sBuffc << endl;
			}


		}



	


	/*
	int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nSocket < 0)
	{
		cout << endl << "Not able to call socket successfully";
		exit(EXIT_FAILURE);
	}

	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	// write your laptop ip address
	//srv.sin_addr.s_addr = inet_addr("192.168.0.105");  // write your laptop ip address
	//srv.sin_addr.s_addr = INADDR_ANY;
	srv.sin_addr.s_addr = inet_addr("192.168.0.105");
	memset(&srv.sin_zero, 0, sizeof(srv));

	nStatus = connect(nSocket, (struct sockaddr*)&srv, sizeof(srv));
	cout << "connect"<<nStatus << endl;
	if (nStatus < 0)
	{
		cout << endl << "connect failed";
		exit(EXIT_FAILURE);
	}
	char sBuffr[1024] = { 0, };
	while (1)
	{
		char sBuff[200];
		char sBuffc[1024] = { 0, };
		cout << endl << "Please enter the message:";
		gets_s(sBuff, 200);

		nStatus = send(nSocket, sBuff, 200, 0);
		if (nStatus < 0)
		{
			cout << endl << "The send failed. Exitting..";
			exit(EXIT_FAILURE);
		}
		else if (nStatus > 0)
		{
			cout << endl << "Message sent!!"<<endl;
		}

		nStatus = recv(nSocket, sBuffc, 200, 0);
		if (nStatus > 0) {
			cout << sBuffc << endl;
		}
		
		
	}
	//nStatus = recv(nSocket, sBuffr, 1024, 0);
	*/

	return 0;
}
