#include "SimpleModelRegistrator.h"
#include <iostream>

SimpleModelRegistrator::SimpleModelRegistrator(float maxError)
{
    this->maxError = maxError;
}

SimpleModelRegistrator::~SimpleModelRegistrator()
{
    //dtor
}

std::list<std::vector<float> > SimpleModelRegistrator::registerModel(std::list<std::vector<float> > basicModel, std::list<std::vector<float> > registeredModel){
    float actualError = maxError+1;
    /*while(actualError > maxError){ //TODO

    }*/

    std::vector<float> mean((*basicModel.begin()).size(), 0);
    std::vector<float> basicMax((*basicModel.begin()).size(), 0);
    std::vector<float> basicMin((*basicModel.begin()).size(), 10000000);
    std::vector<float> regMax((*basicModel.begin()).size(), 0);
    std::vector<float> regMin((*basicModel.begin()).size(), 10000000);

    std::list<std::vector<float> >::iterator basicIt;
    std::list<std::vector<float> >::iterator registeredIt;
    for(basicIt = basicModel.begin(), registeredIt = registeredModel.begin(); basicIt != basicModel.end() && registeredIt != registeredModel.end(); ++basicIt, ++registeredIt){
        for(int i = 0; i<mean.size(); ++i){
            if((*basicIt)[i] < basicMin[i])
                basicMin[i] = (*basicIt)[i];
            if((*basicIt)[i] > basicMax[i])
                basicMax[i] = (*basicIt)[i];

            if((*registeredIt)[i] < regMin[i])
                regMin[i] = (*registeredIt)[i];
            if((*registeredIt)[i] > regMax[i])
                regMax[i] = (*registeredIt)[i];
        }
    }

    std::list<std::vector<float> > scaled;

    for(registeredIt = registeredModel.begin(); registeredIt != registeredModel.end(); ++registeredIt){
        std::vector<float> vec;
        for(int i = 0; i<mean.size(); ++i){
            vec.push_back((*registeredIt)[i]*(basicMax[i]-basicMin[i])/(regMax[i]-regMin[i]));
        }
        scaled.push_back(vec);
    }

    for(basicIt = basicModel.begin(), registeredIt = scaled.begin(); basicIt != basicModel.end() && registeredIt != scaled.end(); ++basicIt, ++registeredIt){
        basicIt++;
        registeredIt++;
        for(int i = 0; i<mean.size(); ++i){
            mean[i] = mean[i] + (*basicIt)[i] - (*registeredIt)[i];
        }
    }

    std::cout << "mean: ";
    for(int i = 0; i<mean.size(); ++i){
        mean[i] = mean[i]/basicModel.size();
        std::cout << mean[i]/basicModel.size() << " ";
    }
    std::cout << std::endl;
    std::list<std::vector<float> > result;

    for(registeredIt = scaled.begin(); registeredIt != scaled.end(); ++registeredIt){
        std::vector<float> vec;
        for(int i = 0; i<mean.size(); ++i){
            vec.push_back((*registeredIt)[i] + mean[i]);
            std::cout << (*registeredIt)[i] + mean[i] << " ";
        }
        result.push_back(vec);
        std::cout << std::endl;
    }

    return result;
}
