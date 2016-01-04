#ifndef FACIALLANDMARKDETECTOR_H
#define FACIALLANDMARKDETECTOR_H
#include <list>
#include <vector>
#include <SFML/Graphics.hpp>
#include <dlib/image_processing.h>
#include <dlib/image_io.h>

using namespace dlib;
using namespace std;

class FacialLandmarkDetector
{
    public:
        FacialLandmarkDetector(){}
        virtual ~FacialLandmarkDetector(){}
        virtual std::list<std::list<std::vector<float> > > getFacesPoints(string imageName) = 0;
        virtual std::list<std::list<std::vector<float> > > getFacesPoints(array2d<rgb_pixel> &image/*, array2d<float> depth*/) = 0;
    protected:
    private:
};

#endif // FACIALLANDMARKDETECTOR_H
