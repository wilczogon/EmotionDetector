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

void FacesDifferencesDatabase::add(Emotion emotion, cv::Mat vec){
    data[emotion].push_back(vec);
    emotions.insert(emotion);
}

cv::Mat FacesDifferencesDatabase::get(Emotion emotion){
    return data[emotion];
}

void FacesDifferencesDatabase::getData(cv::Mat& samples, cv::Mat& responses){
    for(auto emotion: getEmotions())
        for(int rowNo = 0; rowNo < data[emotion].rows; ++rowNo){
            samples.push_back(data[emotion].row(rowNo));
            responses.push_back((int)emotion);
        }

    responses.convertTo(responses, CV_32S);
}

std::set<Emotion> FacesDifferencesDatabase::getEmotions(){
    return emotions;
}

Emotion FacesDifferencesDatabase::getBasicEmotion(){
    return basicEmotion;
}
