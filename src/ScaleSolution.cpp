#include "ScaleSolution.h"

ScaleSolution::ScaleSolution(int dimNo, float requestedDimWidth)
{
    this->dimNo = dimNo;
    this->requestedDimWidth = requestedDimWidth;
}

float ScaleSolution::countDimensionWidth(cv::Mat model, int dimNo)
{
    float minY = model.at<float>(0, dimNo);
    float maxY = minY;

    for(int i = 1; i < model.rows; ++i){
        if(model.at<float>(i, dimNo) < minY)
            minY = model.at<float>(i, dimNo);
        if(model.at<float>(i, dimNo) > maxY)
            maxY = model.at<float>(i, dimNo);
    }

    return maxY - minY;
}

void ScaleSolution::scale(cv::Mat &model, float scale)
{
    model = scale*model;
}

void ScaleSolution::scale(cv::Mat &model, cv::Mat &model2, int dimNo, float dimWidth)
{
    float currDimWidth = (ScaleSolution::countDimensionWidth(model, dimNo) + ScaleSolution::countDimensionWidth(model2, dimNo))/2.0;
    ScaleSolution::scale(model, dimWidth/currDimWidth);
    ScaleSolution::scale(model2, dimWidth/currDimWidth);
}

void ScaleSolution::scale(cv::Mat &model, cv::Mat &model2)
{
    ScaleSolution::scale(model, model2, dimNo, requestedDimWidth);
}
