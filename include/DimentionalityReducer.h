#ifndef DIMENTIONALITYREDUCER_H
#define DIMENTIONALITYREDUCER_H
#include <list>
#include <vector>
#include <opencv2/core.hpp>


class DimentionalityReducer
{
    public:
        DimentionalityReducer(int dimensionsNo);
        virtual ~DimentionalityReducer() = 0;
        virtual void initialize(cv::Mat data) = 0;
        virtual cv::Mat reduceDimentionality(cv::Mat data) = 0;
        int getNumberOfDimensions(){return dimensionsNo;};
    protected:
        int dimensionsNo;
    private:
};

#endif // DIMENTIONALITYREDUCER_H
