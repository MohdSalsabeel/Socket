#include<iostream>
#include<winsock.h>

using namespace std;
#define PORT 9909

struct sockaddr_in srv;
//fd_set fr, fw, fe;
//int nMaxFd;
int nClientSocket;



int main() {

	int nRet = 0;
	// Initialize the WSA variables
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
	{
		//cout << "WSA failed to initialize" << endl;
	}
	else
	{
		//cout << "WSA initialized" << endl;
	}



	// Initialize the socket
	nClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nClientSocket < 0)
	{
		//cout << "The socket is not opened" << endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
	{
		//cout << "The socket opened successfully " << nClientSocket << endl;
	}

	// Initialize the environment for socketaddr structure
	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = inet_addr("127.0.0.1");
	//srv.sin_addr.s_addr = INADDR_ANY;
	memset(&(srv.sin_zero), 0, 8);


	nRet = connect(nClientSocket, (struct sockaddr*)&srv, sizeof(srv));
	if (nRet < 0)
	{
		cout << "Connection failed" << endl;
		WSACleanup();
		return (EXIT_FAILURE);
	}
	else
	{
		cout << "Connected to the Server" << endl;
		
		char buff[255] = { 0 };
		recv(nClientSocket, buff, 255, 0);
		cout << "Just press any key on keyboard to see any message from the server" << endl;
		getchar();
		cout << buff << endl;
		
		// Latest code
		cout << "Now send your message to server" << endl;
		while (1)
		{
			fgets(buff, 256, stdin);
			send(nClientSocket, buff, 256, 0);
			cout<< "Just press any key on keyboard to see any message from the server" << endl;
			getchar();
			recv(nClientSocket, buff, 256, 0);
			cout << buff << endl << "Now send new message" << endl;
		}
	}
	

	


	return 0;
}
