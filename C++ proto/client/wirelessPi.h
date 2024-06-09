#ifndef WIRELESSPI_H
#define WIRELESSPI_H

#include "clientSocket.h"
#include "messageHandler.h"
#include "fileManager.h"
#include "device.h"
#include <string>

using namespace std;

class WirelessPi {
	public:
		WirelessPi();
		void start();
	private:
		ClientSocket socket;
		MessageHandler handler;
		FileManager manager;
		vector<unique_ptr<Device>> devices;
};

#endif
