#pragma once
#pragma warning(disable:4996)
#include <WinSock2.h>
#include <tuple>
#pragma comment(lib, "ws2_32")
#include "packetStructure.h"

class ServerDevice {
public:
	ServerDevice();
	~ServerDevice();
	void initialize();
private:
	WSADATA wsa;
	sockaddr_in serverAddr;
	SOCKET connectSocket;

	void makeThread();
	void recvData();
	int recvn(SOCKET s, char* buf, int len, int flags);

	simplePacket recvSimplePacket();
	shootPacket recvshootPacket();
	posPacket recvposPacket();
};