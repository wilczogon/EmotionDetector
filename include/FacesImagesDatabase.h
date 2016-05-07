#ifndef FACESIMAGESDATABASE_H
#define FACESIMAGESDATABASE_H

#include <Emotion.h>
#include <map>
#include <set>
#include <list>
#include <opencv2/core.hpp>


class FacesImagesDatabase
{
    public:
        FacesImagesDatabase();
        void add(std::string name, std::string personId, Emotion emotion, cv::Mat vec);
        std::map<std::string, cv::Mat> get(std::string personId, Emotion emotion);
        std::set<Emotion> getEmotions();
        std::set<std::string> getPersonIds();
        virtual ~FacesImagesDatabase();
    protected:
    private:
        std::map<std::string, std::map<Emotion, std::map<std::string, cv::Mat> > > data;
        std::set<Emotion> emotions;
        std::set<std::string> personIds;
};

#endif // FACESIMAGESDATABASE_H
