#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include "device.h"

class TemperatureSensor : public Device {
	public:
        TemperatureSensor(string, string, string);
		void update(double) override;
		string prepare(string) override;
	private:

};

#endif
