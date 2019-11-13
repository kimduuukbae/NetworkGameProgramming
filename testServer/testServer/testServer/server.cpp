#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#include "packetDataStructure.h"

#define SERVERPORT 9000
#define BUFSIZE 1024

// 소켓 함수 오류 출력 후 종료
void err_quit(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)& lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)& lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char* buf, int len, int flags)
{
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

DWORD WINAPI recvData(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;

	while (1) {
		packetHead head;
		int headSize = sizeof(head);
		int retval = recvn(client_sock, (char*)& head, headSize, 0);	// head 가 올때까지 기다림
		switch (head.packetType) {
		case E_PACKET_DEGREE: // 만약 각도 변경 요청일경우
			simplePacket pack;
			retval = recvn(client_sock, (char*)& pack, sizeof(pack), 0);
			eventQueue.push(pack);
			break;
		}


	return 0;
}

DWORD WINAPI updataThread(LPVOID arg)
{
	while (1) {
		if (!eventQueue.empty()) {
			auto e = eventQueue.front();
			auto [e1, e2] = e.getPacket();	// 패킷을 열어봄
			if (e1 != nullptr) {
				//e1 에 대한 처리

			}
			else {
				//e2에 대한 처리
			}

			eventQueue.pop();	// eventQueue에서는 이미 처리끝냈으므로 파괴
		}
		updateObject();	// 모든 오브젝트들에 대한 이동, 충돌 처리
// 만약 event Packet 에 대한 처리를 끝냈고, 오브젝트에 대한 업데이트를
		다 끝냈는데  보내야할 데이터가 있다면 sendQueue에 밀어넣음
			sendQueue.push(Event());
	}


	return 0;
}

DWORD WINAPI sendData(LPVOID arg)
{
	if (!sendQueue.empty()) {
		auto e = sendQueue.front();
		auto [e1, e2] = e.getPacket();
		if (e1 != nullptr)
			// simplePacketEvent 라면 이곳에서 처리
		else
			// shootPacketEvent 라면 이곳에서 처리
			sendQueue.pop();

	}
	

	return 0;
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

	HANDLE hThread;

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock[sockNum] = accept(listen_sock, (SOCKADDR*)& clientaddr, &addrlen);
		if (client_sock[sockNum] == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}
		else {
			sockNum++;
			// 접속한 클라이언트 정보 출력
			printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s,포트 번호=%d\n",
				inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		}
	}
	for (int i = 0; i < 3; i++) {
		// 스레드 생성
		hThread = CreateThread(NULL, 0, recvData, (LPVOID)client_sock[i], 0, NULL);
		if (hThread == NULL) { closesocket(client_sock[i]); }
		else { CloseHandle(hThread); }
	}

	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}