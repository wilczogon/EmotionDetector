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

void PCADimentionalityReducer::initialize(std::list<std::vector<float> > data){
    std::cout << "Size of initial matrix, rows: " << Translator::listOfFloatVectorsToMat(data).rows << ", columns: " << Translator::listOfFloatVectorsToMat(data).cols << "\n";
    pca = PCA(Translator::listOfFloatVectorsToMat(data), Mat(), CV_PCA_DATA_AS_ROW, dimensionsNo);
}

std::list<std::vector<float> > PCADimentionalityReducer::reduceDimentionality(std::list<std::vector<float> > data){
    std::list<std::vector<float> > result;

    Mat dataMat = Translator::listOfFloatVectorsToMat(data);
    for(int i = 0; i<dataMat.rows; ++i){
        Mat vec = dataMat.row(i);

        Mat res = pca.project(vec);
        if(res.cols != dimensionsNo)
            throw "PCA cannot reduce dimentionality. Probably too few samples.";
        std::vector<float> v;
        res.copyTo(v);
        result.push_back(v);
    }

    std::cout << "Size of final matrix, rows: " << result.size() << ", columns: " << (*result.begin()).size() << "\n";

    return result;
}

std::vector<float> PCADimentionalityReducer::reduceDimentionality(std::vector<float> vec){
    std::list<std::vector<float> > tmp;
    tmp.push_back(vec);

    return *(reduceDimentionality(tmp).begin());
}
