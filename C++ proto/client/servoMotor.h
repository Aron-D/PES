#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H

#include "device.h"

class ServoMotor : public Device {
	public:
        ServoMotor(string, string, string);
		void update(double) override;
		string prepare(string) override;
	private:
        bool isDoorOpen = 0;
};

#endif
