#ifndef __TANKGAME_APPCONFIG__
#define __TANKGAME_APPCONFIG__

#include <map>
#include <fstream>
#include <string>

#include "../include/utils.h"

using namespace std;

class AppConfig {
private:
	static map<string, string> configValues;
public:

	AppConfig(const char* configFile);

	static void loadConfig(const char* configFile);

	static string getConfig(string configKey);
};



#endif