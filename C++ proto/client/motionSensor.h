#ifndef MOTIONSENSOR_H
#define MOTIONSENSOR_H

#include "device.h"

class MotionSensor : public Device {
	public:
        MotionSensor(string, string, string);
		void update(double) override;
		string prepare(string) override;
	private:
};

#endif
