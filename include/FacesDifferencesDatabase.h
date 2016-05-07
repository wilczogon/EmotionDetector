#ifndef FACESDIFFERENCESDATABASE_H
#define FACESDIFFERENCESDATABASE_H

#include <Emotion.h>
#include <map>
#include <set>
#include <opencv2/core.hpp>

class FacesDifferencesDatabase
{
    public:
        FacesDifferencesDatabase(Emotion basicEmotion);
        void add(std::string name, Emotion emotion, cv::Mat vec);
        cv::Mat get(Emotion emotion);
        std::vector<std::string> getNames(Emotion emotion);
        std::set<Emotion> getEmotions();
        Emotion getBasicEmotion();
        void getData(std::vector<std::string>& names, cv::Mat& samples, cv::Mat& responses);
        virtual ~FacesDifferencesDatabase();
    protected:
    private:
        Emotion basicEmotion;
        std::set<Emotion> emotions;
        std::map<Emotion, std::vector<std::string> > names;
        std::map<Emotion, cv::Mat> data;
};

#endif // FACESDIFFERENCESDATABASE_H
