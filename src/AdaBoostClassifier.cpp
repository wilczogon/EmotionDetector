#include "AdaBoostClassifier.h"

#include <opencv2/core.hpp>
#include <opencv2/ml.hpp>

using namespace cv;

AdaBoostClassifier::AdaBoostClassifier(cv::ml::Boost::Types type, int weakCount, float weightTrimRate, int maxDepth, bool useSurrogates)
{
    boost = cv::ml::Boost::create();
    boost->setBoostType(type);
    boost->setWeakCount(weakCount);
    boost->setWeightTrimRate(weightTrimRate);
    boost->setMaxDepth(maxDepth); // <- crashuje
    boost->setUseSurrogates(useSurrogates);
}

AdaBoostClassifier::~AdaBoostClassifier()
{
    boost.release();
}

void AdaBoostClassifier::initialize(cv::Mat samples, cv::Mat responses){
    Ptr<cv::ml::TrainData> trainData = cv::ml::TrainData::create(samples, cv::ml::SampleTypes::ROW_SAMPLE, responses);
    boost->train(trainData);
    trainData.release();
}

Emotion AdaBoostClassifier::classify(Mat vec){
    return (Emotion)((int)boost->predict(vec));
}
