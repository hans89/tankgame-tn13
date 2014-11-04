
#include "AppConfig.h"

map<string, string> AppConfig::configValues;

AppConfig::AppConfig(const char* configFile) {
  AppConfig::loadConfig(configFile);
}

void AppConfig::loadConfig(const char* configFile) {
  ifstream inStream(configFile);

  string line;

  while (getline(inStream, line).good()) {

    vector<string> configPair = Utils::split(line, '=');

    if (configPair.size() >= 2) {
      configValues.insert(pair<string, string>(
        configPair[0], configPair[1]));
    }
  }
  
  inStream.close();
}

string AppConfig::getConfig(string configKey){
  if (configValues.find(configKey) != configValues.end())
    return configValues[configKey];
  else
    return "";
}

void AppConfig::setConfig(string key, string value){
  configValues[key] = value;
}