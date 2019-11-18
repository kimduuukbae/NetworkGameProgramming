#include <winsock2.h>
#include <iostream>
#include <thread>
#include "stdafx.h"
#include "packetDataStructure.h"
#include "EventManager.h"
#include "ServerDevice.h"

#define SERVERPORT 9000
#define BUFSIZE 1024

// 사용자 정의 데이터 수신 함수
EventManager eventManager;

void recvData(SOCKET s){
	while (1) {
		packetHead head;
		int headSize = sizeof(head);
		int retval = recvn(s, (char*)&head, headSize, 0);	// head 가 올때까지 기다림
		switch (head.packetType) {
		case E_PACKET_SPEED:
			break;
		case E_PACKET_DEGREE: // 만약 각도 변경 요청일경우
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
			auto[simPacket, shtPacket] = e.getPacket();	// 패킷을 열어봄
			if (simPacket != nullptr) {


			}
			else {

			}

		}
		else
			Sleep(1);	// 다른 스레드에게 시간넘겨줌
		//updateObject();	 모든 오브젝트들에 대한 이동, 충돌 처리
		// 만약 event Packet 에 대한 처리를 끝냈고, 오브젝트에 대한 업데이트를 다 끝냈는데  보내야할 데이터가 있다면 sendQueue에 밀어넣음
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

	// 윈속 종료

	return 0;
}