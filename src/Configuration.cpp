#include "Configuration.h"
#include <time.h>

Configuration::Configuration(std::string dataSavePath, bool talkative)
{
    this->dataSavePath = dataSavePath;
    this->talkative = talkative;
    time_t now = time(0);
    tm* localtm = localtime(&now);
    char* tmp = (char*)malloc(100);
    sprintf(tmp, "%00d-%00d-%00d_%00d-%00d-%00d", localtm->tm_year%100, localtm->tm_mon+1, localtm->tm_mday, localtm->tm_hour, localtm->tm_min, localtm->tm_sec);
    runId = std::string(tmp);
}

Configuration::~Configuration()
{
    //dtor
}

bool Configuration::saveVisualizationData(){
    return !dataSavePath.empty();
}

std::string Configuration::getDataSavePath(){
    return dataSavePath;
}

std::string Configuration::getRunId(){
    return runId;
}

bool Configuration::isTalkative(){
    return talkative;
}
