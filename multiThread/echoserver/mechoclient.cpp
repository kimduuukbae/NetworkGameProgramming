#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)
#include <iostream>
#include <winSock2.h>
#include "Error.h"
#include <string>
using namespace std;

int main() {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	auto sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	sockaddr_in serveraddr;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_family = AF_INET;
	int retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	while (1) {
		string s;
		cin >> s;
		if (s.c_str() == "exit")
			break;
		retval = send(sock, s.c_str(), s.size(), 0);
		if (retval == 0)
			break;
		else if (retval == SOCKET_ERROR)
			err_display("send()");
		char BUFF[100];
		retval = recv(sock, BUFF, 100, 0);
		BUFF[retval] = '\0';
		cout << BUFF << endl;
	}
	closesocket(sock);
}