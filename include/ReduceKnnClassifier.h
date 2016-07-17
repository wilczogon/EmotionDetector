#ifndef REDUCEKNNCLASSIFIER_H
#define REDUCEKNNCLASSIFIER_H

#include <Classifier.h>
#include <opencv2/ml.hpp>
#include <opencv2/highgui.hpp>
#include <Visualizer.h>
#include <Configuration.h>


class ReduceKnnClassifier : public Classifier
{
    public:
        ReduceKnnClassifier(int k, Configuration* conf = new Configuration());
        virtual ~ReduceKnnClassifier();
        void initialize(cv::Mat samples, cv::Mat responses);
        Emotion classify(cv::Mat vec);
    protected:
    private:
        int k;
        cv::Ptr<cv::ml::KNearest> knn;
        std::list<std::vector<float> > data;
        Configuration* configuration;
};

#endif // PCAKNNCLASSIFIER_H
