#include "ServerDevice.h"
int main(int argc, char* argv[]){
	device serverDevice;
	serverDevice.initialize();
	serverDevice.acceptClient();
	serverDevice.startServer();

	while (1)
		Sleep(1);

	return 0;
}