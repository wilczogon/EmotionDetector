#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <iostream>
#include <fstream>


class Configuration
{
    public:
        Configuration(std::string dataSavePath = NULL, bool talkative = false);
        virtual ~Configuration();
        bool saveVisualizationData();
        std::string getDataSavePath();
        std::string getRunId();
        bool isTalkative();
        std::ofstream open(std::string contentId);
    protected:
    private:
        std::string dataSavePath;
        std::string runId;
        bool talkative;
};

#endif // CONFIGURATION_H
