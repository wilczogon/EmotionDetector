#include "FacesDifferencesDatabase.h"

#include <opencv2/core.hpp>

FacesDifferencesDatabase::FacesDifferencesDatabase(Emotion basicEmotion)
{
    this->basicEmotion = basicEmotion;
}

FacesDifferencesDatabase::~FacesDifferencesDatabase()
{
    //dtor
}

void FacesDifferencesDatabase::add(std::string name, Emotion emotion, cv::Mat vec){
    data[emotion].push_back(vec);
    names[emotion].push_back(name);
    emotions.insert(emotion);
}

cv::Mat FacesDifferencesDatabase::get(Emotion emotion){
    return data[emotion];
}

std::vector<std::string> FacesDifferencesDatabase::getNames(Emotion emotion){
    return names[emotion];
}

void FacesDifferencesDatabase::getData(std::vector<std::string>& names, cv::Mat& samples, cv::Mat& responses){
    for(auto emotion: getEmotions())
        for(int rowNo = 0; rowNo < data[emotion].rows; ++rowNo){
            samples.push_back(data[emotion].row(rowNo));
            responses.push_back((int)emotion);
            names.push_back("todo");
        }

    responses.convertTo(responses, CV_32F);
    responses.convertTo(responses, CV_32S);
}

std::set<Emotion> FacesDifferencesDatabase::getEmotions(){
    return emotions;
}

Emotion FacesDifferencesDatabase::getBasicEmotion(){
    return basicEmotion;
}
