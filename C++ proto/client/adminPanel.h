#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include "device.h"

class AdminPanel : public Device {
	public:
        AdminPanel(string, string, string);
		void update(double) override;
		string prepare(string) override;
	private:
};

#endif
