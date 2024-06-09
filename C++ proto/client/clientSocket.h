#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <string>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>

using namespace std;

class ClientSocket {
	public:
		ClientSocket();
		string communicate(string, string);
	private:
};

#endif
