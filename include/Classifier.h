#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <Emotion.h>
#include <FacesDifferencesDatabase.h>
#include <opencv2/core.hpp>

class Classifier
{
    public:
        Classifier(){};
        virtual ~Classifier(){};
        virtual void initialize(cv::Mat samples, cv::Mat responses) = 0;
        virtual Emotion classify(cv::Mat vec) = 0;
    protected:
    private:
};

#endif // CLASSIFIER_H
