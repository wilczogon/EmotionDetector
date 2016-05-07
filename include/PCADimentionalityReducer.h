#ifndef PCADIMENTIONALITYREDUCER_H
#define PCADIMENTIONALITYREDUCER_H

#include <DimentionalityReducer.h>
#include <opencv2/core.hpp>

using namespace cv;

class PCADimentionalityReducer : public DimentionalityReducer
{
    public:
        PCADimentionalityReducer(int dimensionsNo);
        virtual ~PCADimentionalityReducer();
        virtual void initialize(cv::Mat data);
        virtual cv::Mat reduceDimentionality(cv::Mat data);
    protected:
    private:
        PCA pca;
};

#endif // PCADIMENTIONALITYREDUCER_H
