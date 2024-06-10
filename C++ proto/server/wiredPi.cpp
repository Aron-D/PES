#include "wiredPi.h"

#define PORT 8080
#define TRUE 1

WiredPi::WirelessPi() {
	socket = new ServerSocket();
	handler = new MessageHandler();
	manager = new FileManager();
	bus = new FieldBus();
}

void WiredPi::start() {

	
	manager.readFromFile(devices);
	socket.init();
	
	while(TRUE) {
		bus.updateAll();
		socket.communicate(bus*);
		handler.handleMessage(devices[i]->get(), receivedMsg);
		manager.writeToFile(devices);			
		usleep(100000);	
	}
}
