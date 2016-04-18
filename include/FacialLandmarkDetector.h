#ifndef FACIALLANDMARKDETECTOR_H
#define FACIALLANDMARKDETECTOR_H
#include <list>
#include <vector>
#include <SFML/Graphics.hpp>
#include <dlib/image_processing.h>
#include <dlib/image_io.h>
#include <opencv2/core.hpp>

using namespace dlib;
using namespace std;

class FacialLandmarkDetector
{
    public:
        FacialLandmarkDetector(){}
        virtual ~FacialLandmarkDetector(){}
        virtual std::list<cv::Mat> getFacesPoints(cv::Mat opencvImage) = 0;
    protected:
    private:
};

#endif // FACIALLANDMARKDETECTOR_H
