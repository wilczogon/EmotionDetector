#ifndef DLIBFACIALLANDMARKDETECTOR_H
#define DLIBFACIALLANDMARKDETECTOR_H

#include <FacialLandmarkDetector.h>
#include <dlib/image_processing.h>
#include <dlib/image_io.h>
#include <dlib/image_processing/frontal_face_detector.h>
//#include <png.h>
#include <opencv2/core.hpp>


class DlibFacialLandmarkDetector : public FacialLandmarkDetector
{
    public:
        DlibFacialLandmarkDetector();
        virtual ~DlibFacialLandmarkDetector();
        virtual std::list<cv::Mat> getFacesPoints(cv::Mat opencvImage);
    protected:
    private:
        frontal_face_detector detector;
        shape_predictor sp;
};

#endif // DLIBFACIALLANDMARKDETECTOR_H
