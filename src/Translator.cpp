#include "Translator.h"
#include <opencv/cv.h>
#include <iostream>

using namespace cv;

std::string Translator::toShort(std::string str){
    return toShort(str, 20);
}

std::string Translator::toShort(std::string str, int k){
    if(str.length() <= k)
        return str;
    else
        return "..." + str.substr(str.length() - k, k);
}

Emotion Translator::toEmotion(std::string name){
    std::string HAPPY = "happy",
            NEUTRAL = "neutral",
            SAD = "sad",
            SURPRISED = "surprised",
            ANGRY = "angry";

    if(name == HAPPY)
        return happy;
    else if(name == NEUTRAL)
        return neutral;
    else if(name == SAD)
        return sad;
    else if(name == SURPRISED)
        return surprised;
    else if(name == ANGRY)
        return angry;
    else
        return (Emotion)NULL;
}

std::string Translator::toString(Emotion emotion){
    std::string HAPPY = "happy",
            NEUTRAL = "neutral",
            SAD = "sad",
            SURPRISED = "surprised",
            ANGRY = "angry";

    if(emotion == Emotion::happy)
        return HAPPY;
    else if(emotion == Emotion::neutral)
        return NEUTRAL;
    else if(emotion == Emotion::sad)
        return SAD;
    else if(emotion == Emotion::surprised)
        return SURPRISED;
    else if(emotion == Emotion::angry)
        return ANGRY;
    else
        return (std::string)NULL;
}

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

std::string Translator::trim(const std::string &s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && isspace(*it))
        it++;

    std::string::const_reverse_iterator rit = s.rbegin();
    while (rit.base() != it && isspace(*rit))
        rit++;

    return std::string(it, rit.base());
}

float depthToMeters(float depthValue)
{
    if (depthValue < 2047)
    {
        return float(1.0 / (depthValue * -0.0030711016 + 3.3309495161));
    }
    return 0.0f;
}

void Translator::translateCoords(cv::Mat coords)
{
    static const double fx_d = 1.0 / 5.9421434211923247e+02;
    static const double fy_d = 1.0 / 5.9104053696870778e+02;
    static const double cx_d = 3.3930780975300314e+02;
    static const double cy_d = 2.4273913761751615e+02;

    //std::cout << fx_d << " " << depthToMeters(coords.at<float>(0, 2)) << " " << (coords.at<float>(0, 0) - cx_d) * depthToMeters(coords.at<float>(0, 2)) * fx_d << std::endl;

    for(int i = 0; i<coords.rows; ++i){
        double depth = depthToMeters(coords.at<float>(i, 2));
        coords.at<float>(i, 0) = float((coords.at<float>(i, 0) - cx_d) * depth * fx_d);
        coords.at<float>(i, 1) = float((coords.at<float>(i, 1) - cy_d) * depth * fy_d);
        coords.at<float>(i, 2) = float(depth);
    }
}

