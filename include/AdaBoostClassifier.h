#ifndef ADABOOSTCLASSIFIER_H
#define ADABOOSTCLASSIFIER_H

#include <Classifier.h>
#include <Emotion.h>
#include <opencv2/core.hpp>
#include <opencv2/ml.hpp>

using namespace cv;

class AdaBoostClassifier: public Classifier
{
    public:
        AdaBoostClassifier(int weakCount, float weightTrimRate, int maxDepth, bool useSurrogates);
        virtual ~AdaBoostClassifier();
        virtual void initialize(cv::Mat samples, cv::Mat responses);
        virtual Emotion classify(Mat vec);
    protected:
    private:
        Ptr<cv::ml::Boost> boost;
};

#endif // ADABOOSTCLASSIFIER_H
