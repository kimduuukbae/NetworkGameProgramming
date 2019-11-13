#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#include "packetDataStructure.h"

#define SERVERPORT 9000
#define BUFSIZE 1024

// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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

// ����� ���� ������ ���� �Լ�
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
		int retval = recvn(client_sock, (char*)& head, headSize, 0);	// head �� �ö����� ��ٸ�
		switch (head.packetType) {
		case E_PACKET_DEGREE: // ���� ���� ���� ��û�ϰ��
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
			auto [e1, e2] = e.getPacket();	// ��Ŷ�� ���
			if (e1 != nullptr) {
				//e1 �� ���� ó��

			}
			else {
				//e2�� ���� ó��
			}

			eventQueue.pop();	// eventQueue������ �̹� ó���������Ƿ� �ı�
		}
		updateObject();	// ��� ������Ʈ�鿡 ���� �̵�, �浹 ó��
// ���� event Packet �� ���� ó���� ���°�, ������Ʈ�� ���� ������Ʈ��
		�� ���´µ�  �������� �����Ͱ� �ִٸ� sendQueue�� �о����
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
			// simplePacketEvent ��� �̰����� ó��
		else
			// shootPacketEvent ��� �̰����� ó��
			sendQueue.pop();

	}
	

	return 0;
}

int main(int argc, char* argv[])
{
	int retval;

	// ���� �ʱ�ȭ
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

	// ������ ��ſ� ����� ����
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
			// ������ Ŭ���̾�Ʈ ���� ���
			printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s,��Ʈ ��ȣ=%d\n",
				inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		}
	}
	for (int i = 0; i < 3; i++) {
		// ������ ����
		hThread = CreateThread(NULL, 0, recvData, (LPVOID)client_sock[i], 0, NULL);
		if (hThread == NULL) { closesocket(client_sock[i]); }
		else { CloseHandle(hThread); }
	}

	// closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}