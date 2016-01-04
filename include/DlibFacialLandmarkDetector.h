#ifndef DLIBFACIALLANDMARKDETECTOR_H
#define DLIBFACIALLANDMARKDETECTOR_H

#include <FacialLandmarkDetector.h>
#include <dlib/image_processing.h>
#include <dlib/image_io.h>
#include <dlib/image_processing/frontal_face_detector.h>


class DlibFacialLandmarkDetector : public FacialLandmarkDetector
{
    public:
        DlibFacialLandmarkDetector();
        virtual ~DlibFacialLandmarkDetector();
        virtual std::list<std::list<std::vector<float> > > getFacesPoints(string imageName);
        virtual std::list<std::list<std::vector<float> > > getFacesPoints(array2d<rgb_pixel> &image/*, array2d<float> depth*/);
    protected:
    private:
        frontal_face_detector detector;
        shape_predictor sp;
};

#endif // DLIBFACIALLANDMARKDETECTOR_H
