#include "ReduceKnnClassifier.h"
#include <Translator.h>
#include <list>

ReduceKnnClassifier::ReduceKnnClassifier(DimentionalityReducer* reducer, int k, bool saveVisualizationData)
{
    this->reducer = reducer;
    this->k = k;
    knn = cv::ml::KNearest::create();
    this->saveVisualizationData = saveVisualizationData;
}

ReduceKnnClassifier::~ReduceKnnClassifier()
{
    //dtor
}

void ReduceKnnClassifier::initialize(FacesDifferencesDatabase* database){
    cv::Mat samples;
    cv::Mat responses;
    std::vector<std::string> names;
    database->getData(names, samples, responses);
    reducer->initialize(samples);
    samples = reducer->reduceDimentionality(samples);
    knn->train(samples, cv::ml::ROW_SAMPLE, responses);
}

Emotion ReduceKnnClassifier::classify(cv::Mat vec){
    cv::Mat response, dist;
    vec = reducer->reduceDimentionality(vec);
    knn->findNearest(vec, k, cv::noArray(), response, dist);

    std::map<int, int> countMap;
    int maxIndex = 0;
    for(int i = 0; i<response.row(0).cols; ++i){
        countMap[i]++;
        if(countMap[i] > response.at<int>(0, i))
            maxIndex = i;
    }

    return (Emotion)response.at<int>(0, maxIndex);
}

/*void ReduceKnnClassifier::visualize(FacesDifferencesDatabase* database, Visualizer* visualizer){
    cv::Mat samples;
    cv::Mat responses;
    std::vector<std::string> names;
    database->getData(names, samples, responses);
    reducer->initialize(samples);
    samples = reducer->reduceDimentionality(samples);
    std::list<sf::Color> colors;
    for(int i = 0; i<responses.cols*responses.rows; ++i)
        colors.push_back(Translator::emotionToColor((Emotion)responses.at<int>(0, i)));
    visualizer->visualizeClusters("Clusters created by ReduceKnnClassifier", samples, colors, 800, 600);
}*/
