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

sf::Color Translator::emotionToColor(Emotion emotion){
    std::string HAPPY = "happy",
            NEUTRAL = "neutral",
            SAD = "sad",
            SURPRISED = "surprised",
            ANGRY = "angry";

    if(emotion == Emotion::happy)
        return sf::Color::Green;
    else if(emotion == Emotion::neutral)
        return sf::Color::Yellow;
    else if(emotion == Emotion::sad)
        return sf::Color::Blue;
    else if(emotion == Emotion::surprised)
        return sf::Color::Magenta;
    else if(emotion == Emotion::angry)
        return sf::Color::Red;
    else
        return sf::Color(10, 10, 10, 255);
}

sf::Color Translator::emotionToColor(void* classPoint){
    std::string HAPPY = "happy",
            NEUTRAL = "neutral",
            SAD = "sad",
            SURPRISED = "surprised",
            ANGRY = "angry";

    Emotion emotion = (Emotion)(*((int*)classPoint));

    if(emotion == Emotion::happy)
        return sf::Color::Green;
    else if(emotion == Emotion::neutral)
        return sf::Color::Yellow;
    else if(emotion == Emotion::sad)
        return sf::Color::Blue;
    else if(emotion == Emotion::surprised)
        return sf::Color::Magenta;
    else if(emotion == Emotion::angry)
        return sf::Color::Red;
    else
        return sf::Color(10, 10, 10, 255);
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

std::list<sf::Color> Translator::classesToColors(std::list<void*> classes, sf::Color(*func)(void*)){
    std::list<sf::Color> colors;

    for(auto classPoint: classes)
        colors.push_back(func(classPoint));

    return colors;
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

