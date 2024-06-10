#ifndef FILEMANAGER_H
#define FILEMANAGER_H

using namespace std;

class FileManager {
	public:
		FileManager();
		bool writeToFile(vector<unique_ptr<Device>>);
		bool readFromFile();
		bool logRFID();
	private:
}

#endif
