#include "ServerDevice.h"

int main(int argc, char* argv[]){
	device serverDevice;
	serverDevice.initialize();
	serverDevice.acceptClient();
	serverDevice.startServer();
	return 0;
}