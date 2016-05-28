#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <iostream>


class Configuration
{
    public:
        Configuration(std::string dataSavePath = NULL, bool talkative = false);
        virtual ~Configuration();
        bool saveVisualizationData();
        std::string getDataSavePath();
        std::string getRunId();
        bool isTalkative();
    protected:
    private:
        std::string dataSavePath;
        std::string runId;
        bool talkative;
};

#endif // CONFIGURATION_H
