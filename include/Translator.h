#ifndef TRANSLATOR_H
#define TRANSLATOR_H
#include <opencv/cv.h>
#include <list>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace cv;

class Translator
{
    public:
        static Mat listOfFloatVectorsToMat(std::list<std::vector<float> > data);
        static std::list<std::vector<float> > matToListOfFloatVectors(Mat matrix);
        static Mat arrayOfFloatsToMat(float* data, int rows, int cols);
        static float* matToArrayOfFloats(Mat matrix);
        static float* listOfFloatVectorsToArrayOfFloats(std::list<std::vector<float> > data);
        static std::list<std::vector<float> > arrayOfFloatsToListOfFloatVectors(float* matrix, int rows, int cols);
        static std::list<sf::Color> classesToColors(std::list<void*> classes, sf::Color(*func)(void*));
    protected:
    private:
};

#endif // TRANSLATOR_H
