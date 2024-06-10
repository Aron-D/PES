#ifndef WIREDPI_H
#define WIREDPI_H

#include "serverSocket.h"
#include "messageHandler.h"
#include "fileManager.h"
#include "device.h" 

using namespace std;

class WiredPi {
	public:
		WiredPi();
		void writeToFile (FILE*, device, const char*, int);
		void readFromFile(FILE*, device, const char*);
		void handleMsg(char*, char*, int, device*, device*);
		void start();
	private:
		ServerSocket socket;
		MessageHandler handler;
		FileManager manager;
		FieldBus bus;
};

#endif
