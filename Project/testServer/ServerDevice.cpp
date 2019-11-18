#include <iostream>
#include "ServerDevice.h"
#include "error.h"



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

	retval = listen(listenSocket, 3);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	std::cout << "Initialized" << std::endl;
}

void ServerDevice::acceptClient(){
	int sockNum = 0;

	while (sockNum != 3) { 
		int addrlen = sizeof(SOCKADDR);
		auto c = accept(listenSocket, (SOCKADDR*)&clientAddr, &addrlen);
		if (clientSocket[sockNum] == INVALID_SOCKET) err_quit("accept()");
		++sockNum;
		std::cout << "클라이언트 접속 : IP = " << inet_ntoa(clientAddr.sin_addr) <<
			",  PORT = " << ntohs(clientAddr.sin_port) << std::endl;
	}
}
