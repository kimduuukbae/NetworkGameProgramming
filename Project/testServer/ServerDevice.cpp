#include <iostream>
#include <thread>
#include "ServerDevice.h"
#include "stdafx.h"
#include "packetDataStructure.h"

ServerDevice::ServerDevice(){
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		err_quit("Startup");
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
}

void ServerDevice::acceptClient(){
	int sockNum = 0;

	while (sockNum != 3) { 
		int addrlen = sizeof(clientAddr);
		clientSocket[sockNum] = accept(listenSocket, (SOCKADDR*)&clientAddr, &addrlen);
		if (clientSocket[sockNum] == INVALID_SOCKET) err_quit("accept()");
		++sockNum;
		std::cout << "Ŭ���̾�Ʈ ���� : IP = " << inet_ntoa(clientAddr.sin_addr) <<
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
		int retval = recvn(s, (char*)&head, headSize, 0);	// head �� �ö����� ��ٸ�
		std::cout << "recvData()" << std::endl;
		switch (head.packetType) {
		case E_PACKET_SPEED:
			break;
		case E_PACKET_DEGREE: // ���� ���� ���� ��û�ϰ��
			simplePacket pack;
			retval = recvn(s, (char*)&pack, sizeof(pack), 0);
			eventManager.pushEvent(pack, E_EVENT);
			break;
		case E_PACKET_SHOOT:
			break;
		case E_PACKET_HIT:
			break;
		case E_PACKET_DIE:
			break;
		}
	}
}

void ServerDevice::updateThread(){
	while (1) {
		if (eventManager.eventQSize()) {
			auto e = eventManager.popEventQueue();
			auto[simPacket, shtPacket] = e.getPacket();	// ��Ŷ�� ���
			if (simPacket != nullptr) {


			}
			else {

			}
		}
		else
			Sleep(1);	// �ٸ� �����忡�� �ð��Ѱ���
		//updateObject();	 ��� ������Ʈ�鿡 ���� �̵�, �浹 ó��
		// ���� event Packet �� ���� ó���� ���°�, ������Ʈ�� ���� ������Ʈ�� �� ���´µ�  �������� �����Ͱ� �ִٸ� sendQueue�� �о����
		//sendQueue.push(Event());
	}
}

void ServerDevice::sendData(){
	while (1) {
		if (eventManager.sendQSize()) {
			auto e = eventManager.popSendQueue();
			auto[simPacket, shtPacket] = e.getPacket();
			if (simPacket != nullptr) {
				packetHead head;
				setPacketHead(head, e);
				if (e.getTarget() == E_EVERYONE) {
					for (int i = 0; i < 3; ++i) {
						send(clientSocket[i], (char*)&head, sizeof(head), 0);
						send(clientSocket[i], (char*)&simPacket, sizeof(simPacket), 0);
					}
				}
				else {
					send(clientSocket[head.id], (char*)&head, sizeof(head), 0);
					send(clientSocket[head.id], (char*)&simPacket, sizeof(simPacket), 0);
				}
			}
			else {
				packetHead head;
				setPacketHead(head, e);
				if (e.getTarget() == E_EVERYONE) {
					for (int i = 0; i < 3; ++i) {
						send(clientSocket[i], (char*)&head, sizeof(head), 0);
						send(clientSocket[i], (char*)&shtPacket, sizeof(shtPacket), 0);
					}
				}
				else {
					send(clientSocket[head.id], (char*)&head, sizeof(head), 0);
					send(clientSocket[head.id], (char*)&shtPacket, sizeof(shtPacket), 0);
				}
			}
		}
		else
			Sleep(1);	// �ٸ� �����忡�� �ð� �Ѱ���
	}
}

void ServerDevice::makeThread(){
	for (int i = 0; i < 3; i++) {
		std::thread{ &ServerDevice::recvData,this,clientSocket[i] }.detach();
		simplePacket s{ i, 0, E_PACKET_SENID };
		eventManager.pushEvent(Event{ s, E_ONE }, E_SEND);
		//��ó�� �� Ŭ���̾�Ʈ�鿡�� �ڽ��� ID�� ������
	}
	std::thread{ &ServerDevice::updateThread,this }.detach();
	std::thread{ &ServerDevice::sendData,this }.detach();
}

void ServerDevice::setPacketHead(packetHead & h, Event& e){
	auto[simPacket, shtPacket] = e.getPacket();
	if (simPacket != nullptr) {
		h.id = simPacket->id;
		h.packetType = simPacket->packetType;
	}
	else {
		h.id = shtPacket->id;
		h.packetType = E_PACKET_SHOOT;
	}
}
