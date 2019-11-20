#include "stdafx.h"
#include "ServerDevice.h"
#include <thread>
#include "ObjectManager.h"
#include "ship.h"
#include <iostream>
ObjectManager* objects = nullptr;
using namespace std;
ServerDevice::ServerDevice(){
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;
}

ServerDevice::~ServerDevice(){

}

void ServerDevice::initialize(){
	connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(9000);
	serverAddr.sin_family = AF_INET;

	connect(connectSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	makeThread();
}

void ServerDevice::makeThread(){
	std::thread{ &ServerDevice::recvData, this }.detach();
}

void ServerDevice::recvData(){

	Ship* myObject = nullptr;
	if (objects == nullptr)
		objects = ObjectManager::instance();
	while (1) {
		packetHead h;

		int retval = recvn(connectSocket, (char*)&h, sizeof(h), 0);
		if (retval == SOCKET_ERROR)
			return;
		cout << (int)h.id << "  " << (int)h.packetType << endl;
		switch (h.packetType) {
		case E_PACKET_SPEED:
			break;
		case E_PACKET_DEGREE:
			break;
		case E_PACKET_SHOOT:
			break;
		case E_PACKET_HIT:
			break;
		case E_PACKET_DIE:
			break;
		case E_PACKET_SENID: {
			simplePacket sim = recvSimplePacket();
			//myObject = objects->getObject<Ship>(h.id);
			break;
		}
		case E_PACKET_OTSET: {
			simplePacket sim = recvSimplePacket();
			////if (sim.id == myObject->getShipIdx())
			//	//break;
			//auto o = objects->getObject<Ship>(sim.id);
			//o->setShipIdx(sim.id);
			//o->setPos(700, 400, 0);
			break;
		}
		}

	}
}

int ServerDevice::recvn(SOCKET s, char * buf, int len, int flags){
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

simplePacket ServerDevice::recvSimplePacket(){
	simplePacket s;
	recvn(connectSocket, (char*)&s, sizeof(s), 0);
	return s;
}

shootPacket ServerDevice::recvshootPacket(){
	shootPacket s;
	recvn(connectSocket, (char*)&s, sizeof(s), 0);
	return s;
}
