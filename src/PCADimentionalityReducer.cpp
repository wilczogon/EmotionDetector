#include "PCADimentionalityReducer.h"
#include "Translator.h"
#include <opencv/cv.h>
#include <iostream>

using namespace cv;

PCADimentionalityReducer::PCADimentionalityReducer()
{
    //ctor
}

PCADimentionalityReducer::~PCADimentionalityReducer()
{
    //dtor
}

void PCADimentionalityReducer::initialize(std::list<std::vector<float> > data, int dimensionsNo){
    this->dimensionsNo = dimensionsNo;

    for(std::vector<float> &vec: data)
        for(float &f: vec)
            std::cout << f << " ";
    std::cout << std::endl;

    std::cout << "Size of Mat: " << Translator::listOfFloatVectorsToMat(data).rows << " " << Translator::listOfFloatVectorsToMat(data).cols << "\n";
    pca = PCA(Translator::listOfFloatVectorsToMat(data), Mat(), CV_PCA_DATA_AS_ROW, dimensionsNo);
}

std::list<std::vector<float> > PCADimentionalityReducer::reduceDimentionality(std::list<std::vector<float> > data){
    std::list<std::vector<float> > result;

    Mat dataMat = Translator::listOfFloatVectorsToMat(data);
    for(int i = 0; i<dataMat.rows; ++i){
        Mat vec = dataMat.row(i);

        Mat res = pca.project(vec);
        std::cout << "Size of Mat(res): " << res.rows << " " << res.cols << "\n";
        if(res.cols != dimensionsNo)
            throw "PCA cannot reduce dimentionality. Probably too few samples.";
        std::vector<float> v;
        res.copyTo(v);
        result.push_back(v);
    }

    return result;
}
