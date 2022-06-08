#include<iostream>
#include<winsock.h>
using namespace std;

int main() {

	WSADATA wsaData;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		return 1;
	}

	struct hostent* remoteHost;
	struct in_addr addr;
	struct sockaddr_in srv;

	char** pAlias;
	
	int nSocket = socket(AF_INET, SOCK_STREAM, 0);

	char szHostName[32] = { 0, };
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
		
		for (pAlias = remoteHost->h_aliases; *pAlias != 0; pAlias)
		{
			cout << *pAlias << endl;
		}
		cout << "Address type: " << endl;
		switch (remoteHost->h_addrtype)
		{
		case AF_INET:
			cout << "AF_INET" << endl;
			srv.sin_family = AF_INET;
			break;
		case AF_NETBIOS:
			cout << "AF_NETBIOS" << endl;
			break;
		default:
			break;
		}

		cout << "Address length: " << remoteHost->h_length << endl;
		int i = 0;
		if (remoteHost->h_addrtype == AF_INET)
		{
			while (remoteHost->h_addr_list[i] != 0)
			{
				addr.s_addr = *(u_long*)remoteHost->h_addr_list[i++];
				cout << inet_ntoa(addr);
			}
		}

		else if (remoteHost->h_addrtype == AF_NETBIOS)
		{
			cout << "NETBIOS address is returned" << endl;
		}

		srv.sin_port = htons(9999);
		srv.sin_addr.s_addr = inet_addr(inet_ntoa(addr));
		memset(&(srv.sin_zero), 0, 8);

		iResult = bind(nSocket, (struct sockaddr*)&srv, sizeof(srv));

		iResult = listen(nSocket, 0);

		

	}


	return 0;
}
