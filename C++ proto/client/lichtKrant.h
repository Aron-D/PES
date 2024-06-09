#ifndef LICHTKRANT_H
#define LICHTKRANT_H

#include "device.h"

class LichtKrant : public Device {
	public:
        LichtKrant(string, string, string);
		void update(double) override;
		string prepare(string) override;
	private:

};

#endif
