#include <winsock2.h>
#include <iostream>
#include <thread>
#include "stdafx.h"
#include "packetDataStructure.h"
#include "EventManager.h"
#include "ServerDevice.h"

#define SERVERPORT 9000
#define BUFSIZE 1024

// ����� ���� ������ ���� �Լ�
EventManager eventManager;

void recvData(SOCKET s){
	while (1) {
		packetHead head;
		int headSize = sizeof(head);
		int retval = recvn(s, (char*)&head, headSize, 0);	// head �� �ö����� ��ٸ�
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

void updateThread(){
	while (1) {
		if (!eventManager.eventQSize()) {
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

void sendData(SOCKET sockets[3]){
	if (!eventManager.sendQSize()) {
		auto e = eventManager.popSendQueue();
		auto [simPacket, shtPacket] = e.getPacket();
		if (simPacket != nullptr) {
			for (int i = 0; i < 3; ++i)
				send(sockets[i], (char*)&simPacket, sizeof(simPacket), 0);
		}
		else {
			for (int i = 0; i < 3; ++i)
				send(sockets[i], (char*)&shtPacket, sizeof(shtPacket), 0);
		}
	}
}

void makeThread(SOCKET sockets[3]) {
	for (int i = 0; i < 3; i++) 
		std::thread{ recvData, sockets[i] }.detach();
	std::thread{ updateThread }.detach();
	std::thread{ sendData, sockets }.detach();
}

int main(int argc, char* argv[]){
	device serverDevice;
	serverDevice.initialize();
	serverDevice.acceptClient();
	//makeThread(client_sock);

	//// closesocket()
	//closesocket(listen_sock);

	// ���� ����

	return 0;
}