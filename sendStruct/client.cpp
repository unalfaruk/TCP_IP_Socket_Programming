#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <thread>
#pragma comment(lib, "ws2_32.lib")

using namespace std;
string ipAddress = "127.0.0.1";			// IP Address of the server
int port = 54000;						// Listening port # on the server
char buffer[160];

int receive_size;
SOCKET sock;

char * client_receive()
{
	if ((receive_size = recv(sock, buffer, 160, 0)) == 0)
	{
		puts("recv failed.");
	}

	puts("reply received\n");

	int * bufintPtr;
	bufintPtr = (int*)&buffer;

	buffer[receive_size] = '\0';
	cout << *bufintPtr << endl;
	//puts(buffer);
	return buffer;
}

int main(int argc, char *argv[])
{

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << WSAGetLastError << endl;
		return 1;
	}
	cout << "Initialized\n";

	// Create socket
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return 1;
	}
	cout << "Created socket\n";


	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		return 1;
	}

	//int intVariable = 64;
	struct myStruct {
		int a;
	};
	myStruct myStruct1;
	myStruct1.a = 3;
	
	int * myPtr = (int*)&myStruct1;


	if (send(sock, (char*)&myStruct1, sizeof(myStruct1), 0) < 0)
	{
		puts("send failed.");
		return 1;
	}
	puts("data send\n");
	thread t1(client_receive);
	t1.join();

	getchar();
	return 0;
}
