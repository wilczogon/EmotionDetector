#include "FacesDifferencesDatabase.h"

FacesDifferencesDatabase::FacesDifferencesDatabase(Emotion basicEmotion)
{
    this->basicEmotion = basicEmotion;
}

FacesDifferencesDatabase::~FacesDifferencesDatabase()
{
    //dtor
}

void FacesDifferencesDatabase::add(Emotion emotion, std::vector<float> vec){
    data[emotion].push_back(vec);
    emotions.insert(emotion);
}

std::list<std::vector<float> > FacesDifferencesDatabase::get(Emotion emotion){
    return data[emotion];
}

std::set<Emotion> FacesDifferencesDatabase::getEmotions(){
    return emotions;
}

Emotion FacesDifferencesDatabase::getBasicEmotion(){
    return basicEmotion;
}
