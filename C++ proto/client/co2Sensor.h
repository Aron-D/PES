#ifndef CO2SENSOR_H
#define CO2SENSOR_H

#include "device.h"

class CO2Sensor : public Device {
	public:
        CO2Sensor(string, string, string);
		void update(double) override;
		string prepare(string) override;
	private:
			
};

#endif
