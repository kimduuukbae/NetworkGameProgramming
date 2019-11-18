#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <iostream>
#include <thread>
#include "stdafx.h"
#include "packetDataStructure.h"
#include "EventManager.h"

#define SERVERPORT 9000
#define BUFSIZE 1024

// 사용자 정의 데이터 수신 함수
EventManager eventManager;

void recvData(SOCKET s)
{
	int retval;

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

void updateThread()
{
	while (1) {
		if (!eventManager.eventQSize()) {
			auto e = eventManager.popEventQueue();
			auto [simPacket, shtPacket] = e.getPacket();	// 패킷을 열어봄
			if (simPacket != nullptr) {
				

			}
			else {
				
			}

		}
		//updateObject();	 모든 오브젝트들에 대한 이동, 충돌 처리
		// 만약 event Packet 에 대한 처리를 끝냈고, 오브젝트에 대한 업데이트를 다 끝냈는데  보내야할 데이터가 있다면 sendQueue에 밀어넣음
		//sendQueue.push(Event());
	}
}

void sendData(SOCKET sockets[3]){
	int retval;

	if (!eventManager.sendQSize()) {
		auto e = eventManager.popSendQueue();
		auto [simPacket, shtPacket] = e.getPacket();
		if (simPacket != nullptr) {
			
			
		}
		else {
			
		}
	}
}

void makeThread(SOCKET sockets[3]) {
	for (int i = 0; i < 3; i++) 
		std::thread{ recvData, sockets[i] }.detach();
	std::thread{ updateThread }.detach();
	std::thread{ sendData, sockets }.detach();
}

int main(int argc, char* argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)& serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock[3];
	int sockNum = 0;
	SOCKADDR_IN clientaddr;
	int addrlen;

	while (sockNum != 3) {
		addrlen = sizeof(clientaddr);
		client_sock[sockNum] = accept(listen_sock, (SOCKADDR*)& clientaddr, &addrlen);
		if (client_sock[sockNum] == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}
		else {
			++sockNum;
			printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s,포트 번호=%d\n",
				inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		}
	}
	makeThread(client_sock);

	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}