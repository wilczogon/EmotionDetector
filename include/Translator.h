#ifndef TRANSLATOR_H
#define TRANSLATOR_H
#include <opencv/cv.h>
#include <list>
#include <vector>
#include <Emotion.h>

using namespace cv;

class Translator
{
    public:
        static std::string toShort(std::string str);
        static std::string toShort(std::string str, int k);
        static Emotion toEmotion(std::string name);
        static std::string toString(Emotion emotion);
        static Mat listOfFloatVectorsToMat(std::list<std::vector<float> > data);
        static std::list<std::vector<float> > matToListOfFloatVectors(Mat matrix);
        static Mat arrayOfFloatsToMat(float* data, int rows, int cols);
        static float* matToArrayOfFloats(Mat matrix);
        static float* listOfFloatVectorsToArrayOfFloats(std::list<std::vector<float> > data);
        static std::list<std::vector<float> > arrayOfFloatsToListOfFloatVectors(float* matrix, int rows, int cols);
        static std::string trim(const std::string &s);
        static void translateCoords(cv::Mat coords);
    protected:
    private:
};

#endif // TRANSLATOR_H
