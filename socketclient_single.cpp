#include <iostream>
#include <winsock.h>
#include <string.h>

using namespace std;

#define PORT 9090

struct sockaddr_in srv;

int main()
{
	WSADATA ws;
	int nStatus = WSAStartup(MAKEWORD(2, 2), &ws);
	if (nStatus < 0)
	{
		cout << endl << "Failed to call WSAStartup";
		exit(EXIT_FAILURE);
	}

	int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nSocket < 0)
	{
		cout << endl << "Not able to call socket successfully";
		exit(EXIT_FAILURE);
	}

	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
  // write your laptop ip address
	srv.sin_addr.s_addr = inet_addr("192.168.0.105");  // write your laptop ip address
	memset(&srv.sin_zero, 0, sizeof(srv));

	nStatus = connect(nSocket, (struct sockaddr*)&srv, sizeof(srv));
	if (nStatus < 0)
	{
		cout << endl << "connect failed";
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		char sBuff[200];
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
			cout << endl << "Message sent!!";
		}
	}

	return 0;
}
