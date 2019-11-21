#pragma once
#include <WinSock2.h>
#include <tuple>
void err_quit(const char* msg);
void err_display(const char* msg);
int recvn(SOCKET s, char* buf, int len, int flags);
int wordToInt(WORD high, WORD lower);
std::tuple<WORD, WORD> intToWord(int value);