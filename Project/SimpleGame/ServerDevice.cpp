#include "stdafx.h"
#include "ServerDevice.h"
#include <thread>
#include "ObjectManager.h"
#include "ship.h"
//#include <iostream>
//using namespace std;
ObjectManager* objects = nullptr;
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

void ServerDevice::sendData(const std::variant<simplePacket, shootPacket, posPacket>& packet){
	packetHead h;
	int packetType = setHeadPacket(packet, h);	
	send(connectSocket, (char*)&h, sizeof(h), 0);
	if (packetType == 0) {
		simplePacket pack = std::get<0>(packet);
		send(connectSocket, (char*)&pack, sizeof(pack), 0);
	}
	else if(packetType == 1) {
		shootPacket pack = std::get<1>(packet);
		send(connectSocket, (char*)&pack, sizeof(pack), 0);
	}
	else if (packetType == 2) {
		posPacket pack = std::get<2>(packet);
		send(connectSocket, (char*)&pack, sizeof(pack), 0);
	}
	else {
		//error!!!!
	}
}

int ServerDevice::getId(){
	return myId;
}

void ServerDevice::makeThread(){
	std::thread{ &ServerDevice::recvData, this }.detach();
}

void ServerDevice::recvData(){

	if (objects == nullptr) 
		objects = ObjectManager::instance(); // lazy initialize
	
	while (1) {
		packetHead h;
		int retval = recvn(connectSocket, (char*)&h, sizeof(h), 0);
		if (retval == SOCKET_ERROR)
			return;
		switch (h.packetType) {
		case E_PACKET_SPEED: {
			simplePacket sim = recvSimplePacket();
			objects->getObject<Ship>(sim.id)->addSpeed(sim.value);
			break;
		}
		case E_PACKET_DEGREE: {
			simplePacket sim = recvSimplePacket();
			objects->getObject<Ship>(sim.id)->rotation(sim.value);
			break;
		}
		case E_PACKET_SHOOT:
			break;
		case E_PACKET_HIT:
			break;
		case E_PACKET_DIE:
			break;
		case E_PACKET_SENID: {
			simplePacket sim = recvSimplePacket();
			myId = h.id;
			break;
		}
		case E_PACKET_OTSET: {
			posPacket pos = recvposPacket();
			auto o = objects->getObject<Ship>(pos.id);
			o->setShipIdx(pos.id);
			o->setPos(pos.posX, pos.posY, 0);
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

posPacket ServerDevice::recvposPacket(){
	posPacket s;
	recvn(connectSocket, (char*)&s, sizeof(s), 0);
	return s;
}

int ServerDevice::setHeadPacket(const std::variant<simplePacket, shootPacket, posPacket>& packet, packetHead& h){
	int count = -1;
	if (auto pack = std::get_if<0>(&packet); pack != nullptr) {
		h.id = pack->id;
		h.packetType = pack->packetType;
		count = 0;
	}
	else if (auto pack = std::get_if<1>(&packet); pack != nullptr) {
		h.id = pack->id;
		h.packetType = E_PACKET_SHOOT;
		count = 1;
	}
	else if (auto pack = std::get_if<2>(&packet); pack != nullptr) {
		h.id = pack->id;
		h.packetType = E_PACKET_OTSET;
		count = 2;
	}
	return count;	//RVO..?
}
