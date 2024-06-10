#ifndef FIELDBUS_H
#define FIELDBUS_H

#include <stdio.h>
#include <string>
#include <memory>
#include <vector>

using namespace std;

class FieldBus
{
	public:
		FieldBus(uint8_t);
		void updateAll();
		string requestVal(string);
	private:
		string i2cDevice;
		int devFd;
		uint8_t slaveID;
		uint8_t message;
		uint8_t ontvangenBytes[4]; 
		vector<unique_ptr<Device>> devices;

};

#endif
