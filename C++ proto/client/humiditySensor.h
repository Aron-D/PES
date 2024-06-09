#ifndef HUMIDITYSENSOR_H
#define HUMIDITYSENSOR_H

#include "device.h"

class HumiditySensor : public Device {
	public:
        HumiditySensor(string, string, string);
		void update(double) override;
		string prepare(string) override;
	private:
};

#endif
