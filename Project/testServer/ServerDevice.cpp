#include <thread>
#include "ServerDevice.h"
#include "stdafx.h"
#include "packetDataStructure.h"
#include <mutex>
#include <iostream>
std::mutex m;
ServerDevice::ServerDevice(){
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		err_quit("Startup");
	eventManager = EventManager::instance();
}
ServerDevice::~ServerDevice(){
	closesocket(listenSocket);
	WSACleanup();
}

void ServerDevice::initialize(){
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET) err_quit("socket()");

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(9000);
	int retval = bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	retval = listen(listenSocket, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	std::cout << "Initialized" << std::endl;
	sendSync = 0.0;
}

void ServerDevice::acceptClient(){
	int sockNum = 0;

	while (sockNum != 3) { 
		int addrlen = sizeof(clientAddr);
		clientSocket[sockNum] = accept(listenSocket, (SOCKADDR*)&clientAddr, &addrlen);
		if (clientSocket[sockNum] == INVALID_SOCKET) err_quit("accept()");
		++sockNum;
		std::cout << "클라이언트 접속 : IP = " << inet_ntoa(clientAddr.sin_addr) <<
			",  PORT = " << ntohs(clientAddr.sin_port) << std::endl;
	}
}

void ServerDevice::startServer(){
	makeThread();
}

void ServerDevice::recvData(SOCKET s){

	while (1) {
		packetHead head;
		int headSize = sizeof(head);
		int retval = recvn(s, (char*)&head, headSize, 0);	// head 가 올때까지 기다림
		switch (head.packetType) {
		case E_PACKET_SPEED: {
			simplePacket pack;
			retval = recvn(s, (char*)&pack, sizeof(pack), 0);
			eventManager->pushEvent(pack, E_EVENT);
			eventManager->pushEvent(pack, E_SEND);
			break;
		}
		case E_PACKET_DEGREE: { // 만약 각도 변경 요청일경우
			simplePacket pack;
			retval = recvn(s, (char*)&pack, sizeof(pack), 0);
			eventManager->pushEvent(pack, E_EVENT);
			eventManager->pushEvent(pack, E_SEND);
			break;
		}
		case E_PACKET_SHOOT:
		{
			shootPacket pack;
			retval = recvn(s, (char*)&pack, sizeof(pack), 0);		
			eventManager->pushEvent(pack, E_EVENT);
			eventManager->pushEvent(pack, E_SEND);
			break;
		}
		case E_PACKET_HIT:
			break;
		case E_PACKET_DIE:
			break;
		}
	}
}

void ServerDevice::updateThread(){
	while (1) {
		if (eventManager->eventQSize()) {
			auto e = eventManager->popEventQueue();
			auto[simPacket, shtPacket, psPacket,aPacket, itmPacket] = e.getPacket();	// 패킷을 열어봄
			if (simPacket != nullptr) {
				Object& o = objectManager.findObject(simPacket->id);
				switch (simPacket->packetType) {
				case E_PACKET_DEGREE:
					o.rotation(simPacket->value);
					break;
				case E_PACKET_SPEED:
					o.addSpeed(simPacket->value);
					break;
				}
			}
			else if(shtPacket != nullptr){
				int idx = objectManager.addObject(value((float)shtPacket->tarPosX, (float)shtPacket->tarPosY, 0.f),
					value((float)shtPacket->mposX, (float)shtPacket->mPosY, 0.f),
					value{ 20.0f,20.0f,1.0f }, E_BULLET);
				auto[id, mx, my, tx, ty] = *shtPacket;
				float vTime = sqrtf(pow(mx - tx, 2) + pow(my - ty, 2)) / 300.0f;
				objectManager.findObject(idx).setVelocity((mx - tx) / vTime, (my - ty) / vTime,0.0f);
				objectManager.findObject(idx).setAncesterIdx(id);
			}
			else if(psPacket != nullptr){

			}
			else {

			}
		}
		deltaTime = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - timePoint).count() / 1000.0;
		if (deltaTime > 0.0159999f) {
			objectManager.update(deltaTime);
			sendSync += deltaTime;
			//std::cout << deltaTime << std::endl;
			if (sendSync > 0.1) {
				sendSync = 0.0;
				for (int i = 0; i < 3; ++i) {
					auto[x, y, z] = objectManager.findObject(i).getPos();
					auto[vx, vy, vz] = objectManager.findObject(i).getVelocity();
					eventManager->pushEvent(allPacket{ (char)i,x,y,vx,vy }, E_SEND);
					//printf("%d %d %d\n", i, (int)x, (int)y);
				}
			}
			timePoint = std::chrono::high_resolution_clock::now();
		}
	}
}

void ServerDevice::sendData(){
	while (1) {
		if (eventManager->sendQSize()) {
			//std::cout << "보낼 데이터 갯수 !!" << eventManager.sendQSize() << std::endl;
			m.lock();
			auto e = eventManager->popSendQueue();
			m.unlock();
			auto[simPacket, shtPacket, psPacket, aPacket, itmPacket] = e.getPacket();
			packetHead head;
			
			setPacketHead(head, e);
			if (simPacket != nullptr) {
				if (e.getTarget() == E_EVERYONE) {
					for (int i = 0; i < 3; ++i) {
						send(clientSocket[i], (char*)&head, sizeof(head), 0);
						send(clientSocket[i], (char*)&simPacket->id, sizeof(simplePacket), 0);
					}
				}
				else {
					send(clientSocket[head.id], (char*)&head, sizeof(head), 0);
					send(clientSocket[head.id], (char*)&simPacket->id, sizeof(simplePacket), 0);
				}
			}
			else if (shtPacket != nullptr){
				if (e.getTarget() == E_EVERYONE) {
					for (int i = 0; i < 3; ++i) {
						send(clientSocket[i], (char*)&head, sizeof(head), 0);
						send(clientSocket[i], (char*)&shtPacket->id, sizeof(shootPacket), 0);
					}
				}
				else {
					send(clientSocket[head.id], (char*)&head, sizeof(head), 0);
					send(clientSocket[head.id], (char*)&shtPacket->id, sizeof(shootPacket), 0);
				}
			}
			else if (psPacket != nullptr){
				if (e.getTarget() == E_EVERYONE) {
					for (int i = 0; i < 3; ++i) {
						send(clientSocket[i], (char*)&head, sizeof(head), 0);
						send(clientSocket[i], (char*)&psPacket->id, sizeof(posPacket), 0);
					}
				}
				else {
					send(clientSocket[head.id], (char*)&head, sizeof(head), 0);
					send(clientSocket[head.id], (char*)&psPacket->id, sizeof(posPacket), 0);
				}
			}
			else if (aPacket != nullptr) {
				for (int i = 0; i < 3; ++i) {
					send(clientSocket[i], (char*)&head, sizeof(head), 0);
					send(clientSocket[i], (char*)&aPacket->id, sizeof(allPacket), 0);
				}
			}
			else {
				for (int i = 0; i < 3; ++i) {
					send(clientSocket[i], (char*)&head, sizeof(head), 0);
					send(clientSocket[i], (char*)&itmPacket->effect, sizeof(itemPacket), 0);
				}
			}
		}
		else
			Sleep(1);	// 다른 쓰레드에게 시간 넘겨줌
	}
}

void ServerDevice::makeThread(){
	objectManager.addObject(value{ -400.0f, -200.0f, 0.0f }, value{ 1.0f,0.0f,0.0f },
		value{ 150.0f,50.0f,1.0f }, E_SHIP);
	objectManager.addObject(value{ -400.0f, 300.0f, 0.0f }, value{ 1.0f,0.0f,0.0f },
		value{ 150.0f,50.0f,1.0f }, E_SHIP);
	objectManager.addObject(value{ 400.0f, -100.0f, 0.0f }, value{ 1.0f,0.0f,0.0f },
		value{ 150.0f,50.0f,1.0f }, E_SHIP);
	timePoint = std::chrono::high_resolution_clock::now();
	objectManager.addObject(value{ 0.f,0.f,0.f }, value{ 0.f,0.f,0.f }, value{ 0.0f,0.0f,0.0f }, E_WIND);
	for (int i = 0; i < 3; i++) {
		std::thread{ &ServerDevice::recvData,this,clientSocket[i] }.detach();
		simplePacket s{ i, 0, E_PACKET_SENID };
		eventManager->pushEvent(Event{ s, E_ONE }, E_SEND);
	}
	posPacket p1{ 0, -400,-200 };
	posPacket p2{ 1, -400,300 };
	posPacket p3{ 2, 400,-100 };

	eventManager->pushEvent(p1, E_SEND);
	eventManager->pushEvent(p2, E_SEND);
	eventManager->pushEvent(p3, E_SEND);

	std::thread{ &ServerDevice::updateThread,this }.detach();

	std::thread{ &ServerDevice::sendData,this }.detach();
}

void ServerDevice::setPacketHead(packetHead & h, Event& e){
	auto[simPacket, shtPacket, posPacket, aPacket, itmPacket] = e.getPacket();
	if (simPacket != nullptr) {
		h.id = simPacket->id;
		h.packetType = simPacket->packetType;
	}
	else if(shtPacket != nullptr){
		h.id = shtPacket->id;
		h.packetType = E_PACKET_SHOOT;
	}
	else if(posPacket != nullptr){
		h.id = posPacket->id;
		h.packetType = E_PACKET_OTSET;
	}
	else if (aPacket != nullptr) {
		h.id = aPacket->id;
		h.packetType = E_PACKET_SYNC;
	}
	else {
		h.id = -1;
		if (itmPacket->effect == -1)
			h.packetType = E_PACKET_WIND;
		else
			h.packetType = E_PACKET_ITEM;
	}
}
