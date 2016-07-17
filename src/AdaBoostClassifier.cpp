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
    //std::cout << "releasing" << std::endl;
    boost.release();
}

void AdaBoostClassifier::initialize(cv::Mat samples, cv::Mat responses){
    //std::cout << "a1 " << samples.rows << " " << samples.cols << " " << std::endl;
    Ptr<cv::ml::TrainData> trainData = cv::ml::TrainData::create(samples, cv::ml::SampleTypes::ROW_SAMPLE, responses);
    //std::cout << "b1 "  << responses.rows << " " << responses.cols << " " << std::endl;
    boost->train(trainData);
    //std::cout << "c1" << std::endl;
    trainData.release();
    //std::cout << "d1" << std::endl;
}

Emotion AdaBoostClassifier::classify(Mat vec){
    return (Emotion)((int)boost->predict(vec));
}
