#include "FacesImagesDatabase.h"

FacesImagesDatabase::FacesImagesDatabase()
{
    //ctor
}

FacesImagesDatabase::~FacesImagesDatabase()
{
    //dtor
}

void FacesImagesDatabase::add(std::string personId, Emotion emotion, std::list<std::vector<float> > vec){
    data[personId][emotion].push_back(vec);
    emotions.insert(emotion);
    personIds.insert(personId);
}

std::list<std::list<std::vector<float> > > FacesImagesDatabase::get(std::string personId, Emotion emotion){
    return data[personId][emotion];
}

std::set<Emotion> FacesImagesDatabase::getEmotions(){
    return emotions;
}

std::set<std::string> FacesImagesDatabase::getPersonIds(){
    return personIds;
}
