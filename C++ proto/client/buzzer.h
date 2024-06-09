#ifndef BUZZER_H
#define BUZZER_H

#include "device.h"

class Buzzer : public Device {
	public:
        Buzzer(string, string, string);
		void update(double) override;
		string prepare(string info) override;
	private:
		bool active;
};

#endif
