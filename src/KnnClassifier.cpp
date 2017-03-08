#include <KnnClassifier.h>
#include <Translator.h>
#include <list>
#include <iostream>
#include <fstream>

KnnClassifier::KnnClassifier(int k, Configuration* conf, std::string id)
{
    this->k = k;
    knn = cv::ml::KNearest::create();
    this->configuration = conf;
    this->id = id;
}

KnnClassifier::~KnnClassifier()
{
    knn.release();
}

void KnnClassifier::initialize(cv::Mat samples, cv::Mat responses){
    knn->train(samples, cv::ml::ROW_SAMPLE, responses);

    if(configuration->saveVisualizationData()){
        std::ofstream* file = configuration->open("KnnClusters_" + id);
        for(int k = 0; k < samples.cols; ++k)
            *file << "x" << k << ", ";
        *file << "expression" << std::endl;
        for(int i = 0; i < samples.rows; i++){
            for(int j = 0; j < samples.cols; j++)
                *file << samples.at<float>(i, j) << ",";
            *file << responses.at<int>(0, i) << std::endl;
        }

        delete file;

        if(configuration->isTalkative())
            std::cout << "Visualization data saved as KnnClusters" << std::endl;
    }
}

Emotion KnnClassifier::classify(cv::Mat vec){
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
        std::ofstream* file = configuration->open("KnnClassify");
        for(int i = 0; i < vec.rows; i++){
            for(int j = 0; j < vec.cols; j++)
                *file << vec.at<float>(i, j) << ",";
            *file << response.at<float>(0, maxIndex) << std::endl;
        }

        delete file;
    }

    return (Emotion)(int)response.at<float>(0, maxIndex);
}
