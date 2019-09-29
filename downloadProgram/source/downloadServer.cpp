#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)
#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "Error.h"
using namespace std;

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

int main() {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	auto listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(9000);

	int retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");
	
	retval = listen(listen_sock, SOMAXCONN);
	if (listen_sock == SOCKET_ERROR) err_quit("listen()");

	while (1) {
		sockaddr_in clientaddr;
		int clientlen = sizeof(clientaddr);
		auto clientsock = accept(listen_sock, (SOCKADDR*)&clientaddr, &clientlen);
		if (clientsock == INVALID_SOCKET) err_display("connect()");

		cout << "���� ����!" << endl;

		// ���� ������ �о����
		int filesize = 0;
		int retval = recvn(clientsock, (char*)&filesize, sizeof(filesize), 0);
		if (retval == SOCKET_ERROR) err_display("recvn()");

		cout << "���� ������ ������� " << filesize << "�Դϴ�." << endl;

		// ���� �̸� �о����,
		//windows ������ ���� �̸� �ִ� ���̴� 260��, 2����Ʈ�� ���� ���尡��
		unsigned short filenameSize;
		retval = recvn(clientsock, (char*)&filenameSize, sizeof(filenameSize), 0);
		
		char* fileName = new char[filenameSize+1];
		retval = recvn(clientsock, fileName, filenameSize, 0);
		fileName[filenameSize] = '\0';
		//���� �ޱ� && ���Ź��� ũ�� �˾ƿ���

		int optval;
		int optlen = sizeof(optval);
		if (getsockopt(clientsock, SOL_SOCKET, SO_RCVBUF,
			(char*)&optval, &optlen) == SOCKET_ERROR)
			err_quit("getsockopt()");

		ofstream out(fileName, ios::binary);
		delete[] fileName;

		char* BUFF = new char[optval];
		float end = filesize;
		float ing = 0;
		while (filesize > 0) {
			if (filesize >= optval)
				retval = recv(clientsock, BUFF, optval, 0);
			else
				retval = recv(clientsock, BUFF, filesize, 0);
			if (retval == 0) {
				cout << "������ ������ϴ�." << endl;
				break;
			}
			filesize -= retval;
			ing += retval;
			out.write(BUFF, retval);
			cout <<floor((ing / end) * 100) << "% �� �޾ҽ��ϴ�." << endl;
		}
		cout << "���� �ٿ�ε尡 ���� �Ǿ����ϴ�." << endl;
		closesocket(clientsock);
		delete[] BUFF;
		break;
	}
	closesocket(listen_sock);

	WSACleanup();
}