#ifndef DEVICE_H
#define DEVICE_H

#include <stdio.h>
#include <String>

using namespace std;

class Device
{
	public:
		Device();
		string getName();
		double getValue();
		virtual void update()=0; 
		uint8_t getAddres();
		uint8_t getMsgFlag();
	private:
		string name;
		double value;
		uint8_t address;
		uint8_t msgFlag;
}

#endif
