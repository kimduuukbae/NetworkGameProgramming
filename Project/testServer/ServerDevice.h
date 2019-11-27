#pragma once
#include <WinSock2.h>
#include <chrono>
#include "EventManager.h"
#include "ObjectManager.h"
#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32")

class ServerDevice {
public:
	ServerDevice();
	~ServerDevice();

	void initialize();
	void acceptClient();
	void startServer();

private:
	SOCKET listenSocket;
	SOCKADDR_IN serverAddr;

	SOCKET clientSocket[3];
	SOCKADDR_IN clientAddr;
	
	WSADATA wsa;

	EventManager eventManager;
	ObjectManager objectManager;
	void recvData(SOCKET s);
	void updateThread();
	void sendData();
	void makeThread();
	void setPacketHead(packetHead& h, Event& e);

	float ItemCreateTime = 5.f;
	float deltaTime;
	float sendSync;
	std::chrono::time_point<std::chrono::steady_clock> timePoint;
};
typedef ServerDevice device;

