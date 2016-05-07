#include "FacesImagesDatabase.h"

#include <opencv2/core.hpp>

FacesImagesDatabase::FacesImagesDatabase()
{
    //ctor
}

FacesImagesDatabase::~FacesImagesDatabase()
{
    //dtor
}

void FacesImagesDatabase::add(std::string name, std::string personId, Emotion emotion, cv::Mat vec){
    data[personId][emotion][name] = vec;
    emotions.insert(emotion);
    personIds.insert(personId);
}

std::map<std::string, cv::Mat> FacesImagesDatabase::get(std::string personId, Emotion emotion){
    return data[personId][emotion];
}

std::set<Emotion> FacesImagesDatabase::getEmotions(){
    return emotions;
}

std::set<std::string> FacesImagesDatabase::getPersonIds(){
    return personIds;
}
