#include "Configuration.h"
#include <time.h>
#include <windows.h>
#include <stdio.h>

Configuration::Configuration(std::string dataSavePath, bool talkative)
{
    this->talkative = talkative;
    time_t now = time(0);
    tm* localtm = localtime(&now);
    char* tmp = (char*)malloc(100);
    sprintf(tmp, "%00d-%00d-%00d_%00d-%00d-%00d", localtm->tm_year%100, localtm->tm_mon+1, localtm->tm_mday, localtm->tm_hour, localtm->tm_min, localtm->tm_sec);
    runId = std::string(tmp);
    this->dataSavePath = dataSavePath + "\\" + runId;
    CreateDirectory(this->dataSavePath.c_str(), NULL);
    std::cout << "All data will be saved under directory: " << this->dataSavePath << std::endl;
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

std::ofstream* Configuration::open(std::string contentId){
    std::ofstream* file;
    std::string filename = contentId + "_" + runId + ".txt";
    file = new std::ofstream(getDataSavePath() + "\\" + filename);
    return file;
}
