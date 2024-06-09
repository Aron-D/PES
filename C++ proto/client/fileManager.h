#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <memory>
#include <vector>
#include "device.h"

using namespace std;

class FileManager {
	public:
		FileManager();
		bool writeToFile(vector<unique_ptr<Device>>, string fileName);
		bool readFromFile(vector<unique_ptr<Device>>, string fileName);
		bool logRFID(string);
	private:
};

#endif
