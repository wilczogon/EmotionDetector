#include "Translator.h"
#include <opencv/cv.h>
#include <iostream>

using namespace cv;

Mat Translator::listOfFloatVectorsToMat(std::list<std::vector<float> > data){
    return arrayOfFloatsToMat(listOfFloatVectorsToArrayOfFloats(data), data.size(), (*data.begin()).size());
}

std::list<std::vector<float> > Translator::matToListOfFloatVectors(Mat matrix){
    return arrayOfFloatsToListOfFloatVectors(matToArrayOfFloats(matrix), matrix.rows, matrix.cols);
}

Mat Translator::arrayOfFloatsToMat(float* data, int rows, int cols){
    return Mat(rows, cols, CV_32F, data);
}

float* Translator::matToArrayOfFloats(Mat matrix){
    return (float*)matrix.data;
}

float* Translator::listOfFloatVectorsToArrayOfFloats(std::list<std::vector<float> > data){
    float* res = (float*)malloc(data.size()*(*data.begin()).size()*sizeof(float));

    //int p = 0;
    int j = 0;
    for(std::vector<float> &vec: data){
        for(int i=0; i<(*data.begin()).size(); ++i)
            res[j*(*data.begin()).size() + i] = vec[i];
        //memcpy(&vec[0], res + p, (*data.begin()).size()*sizeof(float));
        //p += (*data.begin()).size();
        ++j;
    }

    return res;
}

std::list<std::vector<float> > Translator::arrayOfFloatsToListOfFloatVectors(float* matrix, int rows, int cols){
    std::list<std::vector<float> > res;

    for(int i = 0; i<rows; ++i){
        std::vector<float> vec(cols*sizeof(float));
        memcpy(&vec[0], &matrix[i*cols], cols*sizeof(float));
        res.push_back(vec);
    }

    return res;
}

std::list<sf::Color> Translator::classesToColors(std::list<void*> classes, sf::Color(*func)(void*)){
    std::list<sf::Color> colors;

    for(auto classPoint: classes)
        colors.push_back(func(classPoint));

    return colors;
}

