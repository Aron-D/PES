#include "device.h"

Device::Device(){}

Device::string getName(){
	return name;
}
Device::double getValue(){
	return value;
}
Device::void update(double newValue){
	value = newValue;
}
Device::uint8_t getAddres(){
	return address;
}
Device::uint8_t getMsgFlag(){
	return msgFlag;
}
