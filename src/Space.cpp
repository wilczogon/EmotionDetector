#include "Space.h"

Space::Space(std::string fileName){
    //TODO
}

Space::Space(std::list<std::vector<float> > vecs, std::list<void*> classes){
    this->vecs = vecs;
    this->classes = classes;
}

void Space::add(std::vector<float> vec, void* attribute){
    //TODO
}

void Space::save(std::string fileName){
    //TODO
}

void* Space::knn(std::vector<float> vec, int k){
    //TODO
}

std::list<std::vector<float> > Space::getVectors(){
    return vecs;
}

std::list<void*> Space::getClasses(){
    return classes;
}

Space::~Space()
{
    //dtor
}
