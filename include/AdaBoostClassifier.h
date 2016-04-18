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
        AdaBoostClassifier();
        virtual ~AdaBoostClassifier();
        virtual void initialize(FacesDifferencesDatabase* database);
        virtual Emotion classify(Mat vec);
    protected:
    private:
        Ptr<cv::ml::Boost> boost;
};

#endif // ADABOOSTCLASSIFIER_H
