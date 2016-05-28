#ifndef SCALESOLUTION_H
#define SCALESOLUTION_H
#include <opencv2/core.hpp>

class ScaleSolution
{
    public:
        ScaleSolution(int dimNo, float requestedDimWidth);
        static float countDimensionWidth(cv::Mat model, int dimNo);
        static void scale(cv::Mat &model, float scale);
        static void scale(cv::Mat &model, cv::Mat &model2, int dimNo, float dimWidth);
        void scale(cv::Mat &model, cv::Mat &model2);
    protected:
    private:
        int dimNo;
        float requestedDimWidth;
};

#endif // SCALESOLUTION_H
