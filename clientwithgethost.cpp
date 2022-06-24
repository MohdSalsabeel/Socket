#include <iostream>
#include <winsock.h>
#include<stdio.h>
#include <string.h>
#include<conio.h>

using namespace std;

#define PORT 9090

struct sockaddr_in srv;
struct hostent *hp;


//CONCERTO-L158
char szHostName[32] = { 0, };
int iRet = gethostname(szHostName, 32);
char *hostname = szHostName;


int main()
{
	WSADATA ws;
	int nStatus = WSAStartup(MAKEWORD(2, 2), &ws);
	if (nStatus < 0)
	{
		cout << endl << "Failed to call WSAStartup";
		exit(EXIT_FAILURE);
	}

	hp = gethostbyname(hostname);
	

	int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nSocket < 0)
	{
		cout << endl << "Not able to call socket successfully";
		exit(EXIT_FAILURE);
	}

	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	// write your laptop ip address
	//srv.sin_addr.s_addr = inet_addr("10.10.8.159");  // write your laptop ip address
	//srv.sin_addr.s_addr = inet_addr("127.0.0.1");
	srv.sin_addr.s_addr = *((unsigned long *)hp->h_addr);
	//cout<<srv.sin_addr.s_addr<<endl;
	memset(&srv.sin_zero, 0, sizeof(srv));

	nStatus = connect(nSocket, (struct sockaddr*)&srv, sizeof(srv));
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
		//gets_s(sBuff, 200);
		cin>>sBuff;

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
	getch();
	
	return 0;
}
