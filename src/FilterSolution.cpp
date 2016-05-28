#include "FilterSolution.h"
#include <math.h>

FilterSolution::FilterSolution(float lowerThreshold, float upperThreshold)
{
    this->lowerThreshold = lowerThreshold;
    this->upperThreshold = upperThreshold;
}

FilterSolution::~FilterSolution()
{
    //dtor
}

void FilterSolution::filter(cv::Mat &data)
{
    float maxVecLen = FilterSolution::countMaxVectorLen(data);
    for(int i = 0; i<data.rows; ++i){
        float vecLen = 0;
        for(int j = 0; j<data.cols; ++j)
            vecLen += data.at<float>(i, j)*data.at<float>(i, j);
        vecLen = sqrt(vecLen);

        if(vecLen < lowerThreshold*maxVecLen || vecLen > upperThreshold*maxVecLen)
            for(int j = 0; j<data.cols; ++j)
                data.at<float>(i, j) = 0;
    }
}

float FilterSolution::countMaxVectorLen(cv::Mat data)
{
    float maxVecLen = 0;
    for(int i = 0; i<data.rows; ++i){
        float vecLen = 0;
        for(int j = 0; j<data.cols; ++j)
            vecLen += data.at<float>(i, j)*data.at<float>(i, j);
        if(vecLen > maxVecLen)
            maxVecLen = vecLen;
    }

    return sqrt(maxVecLen);
}
