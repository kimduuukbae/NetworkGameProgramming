// 서버는 모든 연결된 클라이언트에게 파일을 전송한다.
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)
#include <iostream>
#include <WinSock2.h>
#include <thread>
#include <fstream>
#include "Error.h"

int recvn(SOCKET s, char* buf, int len, int flags) {
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

void sendFile(SOCKET s) {
	std::cout << "sendFile" << std::endl;
	sockaddr_in clientaddr;
	int clientLen{ sizeof(clientaddr) };
	getpeername(s, (SOCKADDR*)&clientaddr, &clientLen);
	const char* clientName = inet_ntoa(clientaddr.sin_addr);
	unsigned short clientPort{ ntohs(clientaddr.sin_port) };

	std::cout << "IP : " << clientName << "  PORT : " << clientPort << " 접속 " << std::endl;
	
	unsigned short filenameSize;
	int retval = recv(s, (char*)&filenameSize, sizeof(filenameSize), 0);
	if (retval == SOCKET_ERROR)
		err_display("error()");
	filenameSize = ntohs(filenameSize);
	char* BUFF = new char[filenameSize+1];
	retval = recvn(s, BUFF, filenameSize, 0);
	BUFF[filenameSize] = '\0';
	
	std::ifstream in(BUFF, std::ios::in | std::ios::binary | std::ios::ate);
	if (!in) {
		std::cout << "파일이 없습니다." << std::endl;
		closesocket(s);
		return;
	}
	delete[] BUFF;
	int fileSize = in.tellg();
	in.seekg(0, std::ios::beg);
	retval = send(s, (char*)&fileSize, sizeof(fileSize), 0);
	//송신버퍼 크기 얻어옴
	int optval;
	int optlen = sizeof(optval);
	if (getsockopt(s, SOL_SOCKET, SO_SNDBUF,
		(char*)&optval, &optlen) == SOCKET_ERROR)
		err_quit("getsockopt()");
	BUFF = new char[optval];
	float ing = 0;
	float prevval = 0;
	float end = fileSize;
	while (fileSize > 0) {
		in.read(BUFF, optval);
		if (fileSize >= optval)
			retval = send(s, BUFF, optval, 0);
		else
			retval = send(s, BUFF, fileSize, 0);
		if (retval == 0)
			break;
		else if (retval == SOCKET_ERROR)
			err_display("send()");
		fileSize -= retval;
		ing += retval;
		if (auto i = floor((ing / end) * 100); i > prevval) {
			prevval = i;
			std::cout << "IP  : " << clientName << " PORT : " << clientPort <<
				" 에게 " << prevval << " % 를 보냈습니다. " << std::endl;
		}
	}
	in.close();
	delete[] BUFF;
	bool b;
	retval = recv(s, (char*)&b, sizeof(b), 0);
	if (b) {
		closesocket(s);
		std::cout << "접속 종료" << std::endl;
	}
}

int main(int argc, char* argv[]) {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	
	auto listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
		err_quit("socket()");

	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(9000);
	
	int retval = bind(listenSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		err_quit("bind()");

	retval = listen(listenSocket, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		err_quit("listen()");

	while (true) {
		sockaddr_in clientaddr;
		int32_t clientlen{ sizeof(clientaddr) };
		auto clientSocket = accept(listenSocket, (SOCKADDR*)&clientaddr, &clientlen);
		if (clientSocket == SOCKET_ERROR)
			err_quit("accept()");
		std::thread { sendFile, clientSocket }.detach();
	}
	closesocket(listenSocket);
	WSACleanup();
	while (1);
}