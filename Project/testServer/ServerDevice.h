#pragma once
#include <WinSock2.h>
#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32")

class ServerDevice {
public:
	ServerDevice();
	~ServerDevice();

	void initialize();
	void acceptClient();
private:
	SOCKET listenSocket;
	SOCKADDR_IN serverAddr;

	SOCKET clientSocket[3];
	SOCKADDR_IN clientAddr;
	
	WSADATA wsa;
};
typedef ServerDevice device;

