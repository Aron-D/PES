#ifndef DEVICE
#define DEVICE

#include <string>

using namespace std;

class Device {
	public:
		Device(string, string, string);
		string getName();
		string getIp();
		double getValue();
		string getComMSG();
		virtual void update(double) = 0;
		virtual string prepare(string info);
	private:
		string deviceName;
        string ip;
		string message;
protected:
    double value;
};

#endif
