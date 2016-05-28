#include "PCADimentionalityReducer.h"
#include "Translator.h"
#include <opencv/cv.h>
#include <iostream>

using namespace cv;

PCADimentionalityReducer::PCADimentionalityReducer(int dimensionsNo): DimentionalityReducer(dimensionsNo)
{
}

PCADimentionalityReducer::~PCADimentionalityReducer()
{
    //dtor
}

void PCADimentionalityReducer::initialize(cv::Mat data){
    pca = PCA(data, Mat(), CV_PCA_DATA_AS_ROW, dimensionsNo);
}

cv::Mat PCADimentionalityReducer::reduceDimentionality(cv::Mat data){
    return pca.project(data);
}
