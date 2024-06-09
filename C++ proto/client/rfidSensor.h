#ifndef RFIDSENSOR_H
#define RFIDSENSOR_H

#include "device.h"

class RFIDSensor : public Device {
	public:
        RFIDSensor(string, string, string);
		void update(double) override;
		string prepare(string) override;
	private:
};

#endif
