#ifndef PRESSURESENSOR_H
#define PRESSURESENSOR_H

#include "device.h"

class PressureSensor : public Device {
	public:
        PressureSensor(string, string, string);
		void update(double) override;
		string prepare(string) override;
	private:
		static int sequenceTeller;
};

#endif
