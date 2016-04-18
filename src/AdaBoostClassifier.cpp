#include "AdaBoostClassifier.h"

#include <opencv2/core.hpp>
#include <opencv2/ml.hpp>

using namespace cv;

AdaBoostClassifier::AdaBoostClassifier()
{
    boost = cv::ml::Boost::create();
    boost->setBoostType(cv::ml::Boost::REAL);
    boost->setWeakCount(100);
    boost->setWeightTrimRate(0.95);
    boost->setMaxDepth(2);
    boost->setUseSurrogates(false);
    boost->setPriors(Mat());
}

AdaBoostClassifier::~AdaBoostClassifier()
{
    //dtor
}

void AdaBoostClassifier::initialize(FacesDifferencesDatabase* database){
    cv::Mat samples;
    cv::Mat responses;
    database->getData(samples, responses);
    Ptr<cv::ml::TrainData> trainData = cv::ml::TrainData::create(samples, cv::ml::SampleTypes::ROW_SAMPLE, responses);
    boost->train(trainData);
}

Emotion AdaBoostClassifier::classify(Mat vec){
    return (Emotion)((int)boost->predict(vec));
}
