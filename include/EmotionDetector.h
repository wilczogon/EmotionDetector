#define _WINDOWS

#ifndef EMOTIONDETECTOR_H
#define EMOTIONDETECTOR_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Emotion.h"
#include "Space.h"
#include "ModelRegistrator.h"
#include <Classifier.h>
#include <FacesImagesDatabase.h>
#include <FacesDifferencesDatabase.h>
#include <opencv2/core.hpp>


class EmotionDetector
{
    public:
        EmotionDetector(ModelRegistrator* registrator, Classifier* classifier);
        virtual ~EmotionDetector();
        void initialize(FacesImagesDatabase* database, Emotion basicEmotion);
        Emotion classify(cv::Mat basicExpression, cv::Mat specialExpression);
        float test(FacesImagesDatabase* testDatabase, bool loggingOn);
    protected:
    private:
        cv::Mat countDifference(cv::Mat basicFaceExpression, cv::Mat specialFaceExpression);
        FacesDifferencesDatabase* prepareDiffs(FacesImagesDatabase* database, Emotion basicEmotion);
        ModelRegistrator* registrator;
        Classifier* classifier;
        FacesDifferencesDatabase* diffs;
};

#endif // EMOTIONDETECTOR_H
