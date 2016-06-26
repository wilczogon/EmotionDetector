#ifndef TRANSLATOR_H
#define TRANSLATOR_H
#include <opencv/cv.h>
#include <list>
#include <vector>
//#include <SFML/Graphics.hpp>
#include <Emotion.h>

using namespace cv;

class Translator
{
    public:
        static std::string toShort(std::string str);
        static std::string toShort(std::string str, int k);
        //static sf::Color emotionToColor(Emotion emotion);
        //static sf::Color emotionToColor(void* classPoint);
        static Emotion toEmotion(std::string name);
        static std::string toString(Emotion emotion);
        static Mat listOfFloatVectorsToMat(std::list<std::vector<float> > data);
        static std::list<std::vector<float> > matToListOfFloatVectors(Mat matrix);
        static Mat arrayOfFloatsToMat(float* data, int rows, int cols);
        static float* matToArrayOfFloats(Mat matrix);
        static float* listOfFloatVectorsToArrayOfFloats(std::list<std::vector<float> > data);
        static std::list<std::vector<float> > arrayOfFloatsToListOfFloatVectors(float* matrix, int rows, int cols);
        //static std::list<sf::Color> classesToColors(std::list<void*> classes, sf::Color(*func)(void*));
        static std::string trim(const std::string &s);
    protected:
    private:
};

#endif // TRANSLATOR_H
