#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)
#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include "Error.h"
using namespace std;

int main(int argc, char* argv[]) {
	ifstream in(argv[3], ios::binary | ios::in);
	if (!in) {
		cout << "파일이 없습니다." << endl;
		return 1;
	}
	in.seekg(0, ios::end);
	int filesize = in.tellg();	// 파일은 몇 바이트?
	in.seekg(0, ios::beg);

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	auto sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(stoi(argv[2]));

	int retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	cout << "파일은 " << filesize << "바이트 입니다." << endl;
	retval = send(sock, (char*)&filesize, sizeof(filesize), 0);

	unsigned short filenameSize = strlen(argv[3]);
	retval = send(sock, (char*)&filenameSize, 2, 0);
	if (retval == SOCKET_ERROR || !retval)
		err_quit("send() filenameSize");

	retval = send(sock, argv[3], filenameSize, 0);
	if (retval == SOCKET_ERROR || !retval)
		err_quit("send() filename");

	// 송신버퍼 크기 알아오기
	// SOL_SOCKET : 옵션을 해석하고 처리하는 코드를 나타냄 (옵션레벨)

	int optval;
	int optlen = sizeof(optval);
	if (getsockopt(sock, SOL_SOCKET, SO_SNDBUF,
		(char*)&optval, &optlen) == SOCKET_ERROR)
		err_quit("getsockopt()");

	char* BUFF = new char[optval];
	float ing = 0;
	float end = filesize;
	while (filesize > 0) {
		in.read(BUFF, optval);
		if(filesize >= optval)
			retval = send(sock, BUFF, optval, 0);
		else
			retval = send(sock, BUFF, filesize, 0);
		filesize -= retval;
		ing += retval;
		cout << floor((ing / end) * 100) << "% 를 보냈습니다." << endl;
	}
	cout << "모든 파일을 보냈습니다. " << endl <<"파일 전송이 종료되었습니다." << endl;
	closesocket(sock);
	delete[] BUFF;
	WSACleanup();
}