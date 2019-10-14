// 모든 클라는, 서버에서 파일을 다운로드 받아 온다.
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)
#include <iostream>
#include <WinSock2.h>
#include <string>
#include <fstream>
#include "Error.h"
using namespace std;

int main(int argc, char* argv[]) {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	auto sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in serveraddr;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(stoi(argv[2]));
	serveraddr.sin_family = AF_INET;

	int retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		err_quit("connect()");
	unsigned short size = htons(strlen(argv[3]));
	send(sock, (char*)&size, sizeof(size), 0);

	send(sock, argv[3], size, 0);
	
	int optval;
	int optlen = sizeof(optval);
	if (getsockopt(sock, SOL_SOCKET, SO_RCVBUF,
		(char*)&optval, &optlen) == SOCKET_ERROR)
		err_quit("getsockopt()");

	int fileSize = 0;
	retval = recv(sock, (char*)&fileSize, sizeof(fileSize), 0);
	cout << fileSize << "크기의 파일입니다." << endl;

	ofstream out(argv[3], std::ios::trunc | std::ios::binary);
	char* BUFF = new char[optval];
	float ing = 0;
	float prevval = 0;
	float end = fileSize;
	while (fileSize > 0) {
		if (fileSize >= optval)
			retval = recv(sock, BUFF, optval, 0);
		else
			retval = recv(sock, BUFF, fileSize, 0);
		if (retval == 0 || retval == SOCKET_ERROR) {
			err_quit("send()");
			break;
		}
		if (auto i = floor((ing / end) * 100); i > prevval) {
			prevval = i;
			cout << prevval << " % 받았습니다.  " << endl;
		}
		fileSize -= retval;
		ing += retval;
		out.write(BUFF, retval);
	}
	bool bRecv = true;
	retval = send(sock, (char*)&bRecv, sizeof(bRecv), 0);
	cout << "100 % 받았습니다. 종료합니다." << endl;
	delete[] BUFF;
	closesocket(sock);
	WSACleanup();
}