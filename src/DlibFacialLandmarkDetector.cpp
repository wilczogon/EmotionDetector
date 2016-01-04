#include "DlibFacialLandmarkDetector.h"
#include <dlib/image_processing.h>
#include <dlib/image_io.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/geometry.h>

DlibFacialLandmarkDetector::DlibFacialLandmarkDetector()
{
    detector = get_frontal_face_detector();
    deserialize("shape_predictor_68_face_landmarks.dat") >> sp;
}

DlibFacialLandmarkDetector::~DlibFacialLandmarkDetector()
{
    //dtor
}

std::list<std::list<std::vector<float> > > DlibFacialLandmarkDetector::getFacesPoints(string imageName){
    array2d<rgb_pixel> img;
    load_image(img, imageName);
    return getFacesPoints(img);
}

std::list<std::list<std::vector<float> > > DlibFacialLandmarkDetector::getFacesPoints(array2d<rgb_pixel> &image/*, array2d<float> depth*/){
    std::list<std::list<std::vector<float> > > result;
    //pyramid_up(image);
    std::vector<rectangle> dets = detector(image);

    for (unsigned long j = 0; j < dets.size(); ++j){
        std::list<std::vector<float> > facePoints;
        full_object_detection shape = sp(image, dets[j]);
        for(int i = 0; i<shape.num_parts(); ++i){
            std::vector<float> point;
            point.push_back((shape.part(i).x()));
            point.push_back((shape.part(i).y()));
            facePoints.push_back(point);
        }

        result.push_back(facePoints);
    }

    return result;
}
