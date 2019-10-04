#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32")
#include <iostream>
#include <winSock2.h>
#include <thread>
#include "Error.h"

void exchangeData(SOCKET s) {
	while (1) {
		char BUFF[100];
		int retval = recv(s, BUFF, 100, 0);
		if (!retval)
			break;
		else if (retval == SOCKET_ERROR)
			err_display("send()");
		BUFF[retval] = '\0';
		std::cout << BUFF << std::endl;
		retval = send(s, BUFF, retval, 0);
	}
	closesocket(s);
}
int main() {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	auto listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET) err_quit("socket()");

	sockaddr_in serveraddr;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_family = AF_INET;

	int retval = bind(listenSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	retval = listen(listenSocket, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	while (1) {
		SOCKET clientSocket;
		sockaddr_in clientaddr;
		int length{ sizeof(clientaddr) };
		clientSocket = accept(listenSocket, (SOCKADDR*)&clientaddr, &length);
		if (clientSocket == INVALID_SOCKET) err_display("accept()");
		std::thread{ exchangeData, clientSocket }.detach();
	}
	closesocket(listenSocket);
}