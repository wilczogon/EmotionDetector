#include "ReduceKnnClassifier.h"
#include <Translator.h>
#include <list>
#include <iostream>
#include <fstream>

ReduceKnnClassifier::ReduceKnnClassifier(int k, Configuration* conf)
{
    this->k = k;
    knn = cv::ml::KNearest::create();
    this->configuration = conf;
}

ReduceKnnClassifier::~ReduceKnnClassifier()
{
    knn.release();
}

void ReduceKnnClassifier::initialize(cv::Mat samples, cv::Mat responses){
    knn->train(samples, cv::ml::ROW_SAMPLE, responses);

    if(configuration->saveVisualizationData()){
        std::ofstream file;
        std::string filename = "ReduceKnnClusters_" + configuration->getRunId() + ".txt";
        file.open(configuration->getDataSavePath() + "\\" + filename);
        for(int i = 0; i < samples.rows; i++){
            for(int j = 0; j < samples.cols; j++)
                file << samples.at<float>(i, j) << ";";
            file << responses.at<int>(0, i) << std::endl;
        }

        file.close();

        if(configuration->isTalkative())
            std::cout << "Visualization data saved as " << filename << std::endl;
    }
}

Emotion ReduceKnnClassifier::classify(cv::Mat vec){
    cv::Mat response, dist;
    knn->findNearest(vec, k, cv::noArray(), response, dist);

    std::map<int, int> countMap;
    int maxIndex = 0;
    for(int i = 0; i<response.cols; ++i){
        countMap[(int)response.at<float>(i)]++;
        if(countMap[(int)response.at<float>(0, i)] > countMap[(int)response.at<float>(0, maxIndex)])
            maxIndex = i;
    }

    if(configuration->saveVisualizationData()){
        std::ofstream file;
        std::string filename = "ReduceKnnClassify_" + configuration->getRunId() + ".txt";
        file.open(configuration->getDataSavePath() + "\\" + filename);
        for(int i = 0; i < vec.rows; i++){
            for(int j = 0; j < vec.cols; j++)
                file << vec.at<float>(i, j) << ";";
            file << response.at<float>(0, maxIndex) << std::endl;
        }

        file.close();
    }

    return (Emotion)(int)response.at<float>(0, maxIndex);
}
