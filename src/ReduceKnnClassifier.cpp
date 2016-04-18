#include "ReduceKnnClassifier.h"

ReduceKnnClassifier::ReduceKnnClassifier(DimentionalityReducer* reducer)
{
    this->reducer = reducer;
}

ReduceKnnClassifier::~ReduceKnnClassifier()
{
    //dtor
}

void ReduceKnnClassifier::initialize(FacesDifferencesDatabase* database){
    //reducer->initialize(vecs);
    //this->data = reducer->reduceDimentionality(vecs);
}

Emotion ReduceKnnClassifier::classify(std::vector<float> vec){
    vec = reducer->reduceDimentionality(vec);
    //TODO classify
}
