#include "../includes/SimConfig.h"

SimConfig::SimConfig(string filepath) {}

SimConfig * SimConfig::instance(nullptr);

SimConfig * SimConfig::getInstance(string filepath) {
    if (instance == nullptr) {
        instance = new SimConfig(filepath);
        ifstream file(filepath);
        instance->params = json::parse(file);
    }
    return instance;
}

SimConfig * SimConfig::getInstanceNew(string filepath) {
    if (instance == nullptr) {
        instance = new SimConfig("nada");
        instance->params = json::parse(filepath);
    }
    return instance;
}

json SimConfig::getParams() {
    return this->instance->params;
}
