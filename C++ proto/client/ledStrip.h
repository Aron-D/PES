#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include "device.h"

class LEDStrip : public Device {
	public:
        LEDStrip(string, string, string);
		void update(double) override;
		string prepare(string info) override;
	private:
};

#endif
