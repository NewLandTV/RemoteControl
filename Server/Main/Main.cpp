#include <iostream>
#include <WinSock2.h>

#define SERVER_PORT 7234
#define MAX_BUFFER_SIZE 1024

#define GotoXY(x, y) (SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x, y }))

#pragma comment(lib, "ws2_32.lib")

int main()
{
	// Console initialize
	system("title Remote Control");
	system("mode con cols=60 lines=50");

	CONSOLE_CURSOR_INFO consoleCursorInfo;

	consoleCursorInfo.bVisible = 0;
	consoleCursorInfo.dwSize = 1;

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleCursorInfo);

	// WinSock v2.2 initialize
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cout << "WSAStartup Error!";

		return 1;
	}

	// Socket initialize
	SOCKET listeningSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (listeningSocket == INVALID_SOCKET)
	{
		std::cout << "socket Error!";

		WSACleanup();

		return 1;
	}

	// Bind & Listen
	SOCKADDR_IN serverAddrIn;

	serverAddrIn.sin_family = AF_INET;
	serverAddrIn.sin_port = htons(SERVER_PORT);
	serverAddrIn.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	if (bind(listeningSocket, (SOCKADDR*)&serverAddrIn, sizeof(serverAddrIn)) == SOCKET_ERROR)
	{
		std::cout << "bind Error!";

		closesocket(listeningSocket);
		WSACleanup();

		return 1;
	}

	if (listen(listeningSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << "listen Error!";

		closesocket(listeningSocket);
		WSACleanup();

		return 1;
	}

	// Accept
	SOCKADDR_IN clientAddrIn;
	int clientAddrInLength = sizeof(clientAddrIn);
	SOCKET clientSocket = accept(listeningSocket, (SOCKADDR*)&clientAddrIn, &clientAddrInLength);

	closesocket(listeningSocket);

	if (clientSocket == INVALID_SOCKET)
	{
		std::cout << "accept Error!";

		WSACleanup();

		return 1;
	}

	// Send & Receive
	char buffer[MAX_BUFFER_SIZE];

	while (true)
	{
		ZeroMemory(buffer, MAX_BUFFER_SIZE);

		int receiveBytes = recv(clientSocket, buffer, MAX_BUFFER_SIZE, 0);

		if (receiveBytes == SOCKET_ERROR)
		{
			std::cout << "recv Error!";

			break;
		}
		else if (receiveBytes == 0)
		{
			std::cout << "Client disconnected";

			break;
		}

		GotoXY(30, 25);

		std::cout << (buffer[0] == 'T' ? "+" : " ") << std::endl;

		send(clientSocket, buffer, receiveBytes + 1, 0);
	}

	// Close
	closesocket(clientSocket);
	WSACleanup();

	return 0;
}