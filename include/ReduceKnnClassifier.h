#ifndef REDUCEKNNCLASSIFIER_H
#define REDUCEKNNCLASSIFIER_H

#include <Classifier.h>
#include <DimentionalityReducer.h>
#include <opencv2/ml.hpp>
#include <opencv2/highgui.hpp>
#include <Visualizer.h>
#include <Configuration.h>


class ReduceKnnClassifier : public Classifier
{
    public:
        ReduceKnnClassifier(DimentionalityReducer* reducer, int k, Configuration* conf = new Configuration());
        virtual ~ReduceKnnClassifier();
        void initialize(FacesDifferencesDatabase* database);
        Emotion classify(cv::Mat vec);
        //void visualize(FacesDifferencesDatabase* database, Visualizer* visualizer);
    protected:
    private:
        int k;
        cv::Ptr<cv::ml::KNearest> knn;
        DimentionalityReducer* reducer;
        std::list<std::vector<float> > data;
        bool saveVisualizationData;
        Configuration* configuration;
};

#endif // PCAKNNCLASSIFIER_H
