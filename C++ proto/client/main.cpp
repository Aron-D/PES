#include "wirelessPi.h"

int main(int argc, char *argv[]) {
	WirelessPi* pi = new WirelessPi();
	pi->start();
	return 0;
}
