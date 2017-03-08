#include "FilterSolution.h"
#include <math.h>
#include <iostream>

FilterSolution::FilterSolution(float lowerThreshold, float upperThreshold)
{
    isInitialized = false;
    this->lowerThreshold = lowerThreshold;
    this->upperThreshold = upperThreshold;
}

FilterSolution::~FilterSolution()
{
    //dtor
}

void FilterSolution::initialize(cv::Mat &data)
{
    maxVectorLength = 0;
    for(int i = 0; i<data.rows; ++i){
        for(int j = 0; j<data.cols; ++j){
            float vecLen = sqrt(data.at<float>(i, j)*data.at<float>(i, j));
            //std::cout << vecLen << " " << data.at<float>(i, j) << std::endl;
            if(vecLen > maxVectorLength)
                maxVectorLength = vecLen;
        }
    }
    isInitialized = true;
    //std::cout << maxVectorLength << std::endl;
}

void FilterSolution::filter(cv::Mat &data)
{
    if(!isInitialized)
        return;

    //float maxVecLen = FilterSolution::countMaxVectorLen(data);
    for(int i = 0; i<data.rows; ++i){
        for(int j = 0; j<data.cols; ++j){
            float vecLen = sqrt(data.at<float>(i, j)*data.at<float>(i, j));

            if(vecLen < lowerThreshold*maxVectorLength || vecLen > upperThreshold*maxVectorLength)
                data.at<float>(i, j) = 0;
        }
    }
}

/*float FilterSolution::countMaxVectorLen(cv::Mat data)
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
}*/
