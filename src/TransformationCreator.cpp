#include "TransformationCreator.h"

TransformationCreator::TransformationCreator()
{
}

TransformationCreator::~TransformationCreator()
{
    //dtor
}

Transformation* TransformationCreator::createOptimalTransformation(std::list<std::vector<float> > vecs){
    return new Transformation(); //TODO
}

std::vector<float> TransformationCreator::applyTransformation(std::vector<float> vec, Transformation* transformation){
    //TODO
    return vec;
}
