#pragma once

void err_quit(char* msg);
void err_display(char* msg);
int recvn(unsigned int s, char* buf, int len, int flags);