#define _WINDOWS

#ifndef EMOTIONDETECTOR_H
#define EMOTIONDETECTOR_H
#include <vector>
#include <list>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Emotion.h"
#include "Space.h"
#include "ModelRegistrator.h"
#include <Classifier.h>
#include <FacesImagesDatabase.h>
#include <FacesDifferencesDatabase.h>

class EmotionDetector
{
    public:
        EmotionDetector(ModelRegistrator* registrator, Classifier* classifier);
        virtual ~EmotionDetector();
        void initialize(FacesImagesDatabase* database, Emotion basicEmotion);
        Emotion classify(std::list<std::vector<float> > basicExpression, std::list<std::vector<float> > specialExpression);
        float test(FacesImagesDatabase* testDatabase, bool loggingOn);
        //Space* createSpaceOfDifferences(std::list<std::vector<float> > differences, std::list<Emotion> emotions);
    protected:
    private:
        std::vector<float> countDifference(std::list<std::vector<float> > basicFaceExpression, std::list<std::vector<float> > specialFaceExpression);
        FacesDifferencesDatabase* prepareDiffs(FacesImagesDatabase* database, Emotion basicEmotion);
        ModelRegistrator* registrator;
        Classifier* classifier;
        FacesDifferencesDatabase* diffs;
};

#endif // EMOTIONDETECTOR_H
