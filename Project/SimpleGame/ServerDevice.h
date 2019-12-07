#pragma once
#pragma warning(disable:4996)
#include <WinSock2.h>
#include <variant>
#pragma comment(lib, "ws2_32")
#include "packetStructure.h"

class ServerDevice {
public:
	ServerDevice();
	~ServerDevice();
	void initialize();
	void sendData(const std::variant<simplePacket,shootPacket,posPacket, allPacket>& packet);
	int getId();
	int getLive();
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
	allPacket recvallPacket();
	itemPacket recvItemPacket();

	int setHeadPacket(const std::variant<simplePacket, shootPacket, posPacket, allPacket>& packet, packetHead& h);

	int myId;
	int live = 3;
};