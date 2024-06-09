#ifndef MOISTURESENSOR_H
#define MOISTURESENSOR_H

#include "device.h"

class MoistureSensor : public Device {
	public:
        MoistureSensor(string, string, string);
		void update(double) override;
		string prepare(string) override;
	private:

};

#endif
