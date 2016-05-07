#include "AdaBoostClassifier.h"

#include <opencv2/core.hpp>
#include <opencv2/ml.hpp>

using namespace cv;

AdaBoostClassifier::AdaBoostClassifier(int weakCount, float weightTrimRate, int maxDepth, bool useSurrogates)
{
    boost = cv::ml::Boost::create();
    boost->setBoostType(cv::ml::Boost::REAL);
    boost->setWeakCount(weakCount);
    boost->setWeightTrimRate(weightTrimRate);
    boost->setMaxDepth(maxDepth);
    boost->setUseSurrogates(useSurrogates);
    boost->setPriors(Mat());
}

AdaBoostClassifier::~AdaBoostClassifier()
{
    //dtor
}

void AdaBoostClassifier::initialize(FacesDifferencesDatabase* database){
    cv::Mat samples;
    cv::Mat responses;
    std::vector<std::string> names;
    database->getData(names, samples, responses);
    Ptr<cv::ml::TrainData> trainData = cv::ml::TrainData::create(samples, cv::ml::SampleTypes::ROW_SAMPLE, responses);
    boost->train(trainData);
}

Emotion AdaBoostClassifier::classify(Mat vec){
    return (Emotion)((int)boost->predict(vec));
}
