#define _WINDOWS

#ifndef EMOTIONDETECTOR_H
#define EMOTIONDETECTOR_H
#include <iostream>
#include "Emotion.h"
#include "Space.h"
#include "ModelRegistrator.h"
#include <Classifier.h>
#include <FacesImagesDatabase.h>
#include <FacesDifferencesDatabase.h>
#include <opencv2/core.hpp>
#include <ScaleSolution.h>
#include <FilterSolution.h>
#include <Configuration.h>
#include <DimentionalityReducer.h>


class EmotionDetector
{
    public:
        EmotionDetector(
                        ModelRegistrator* registrator,
                        DimentionalityReducer* reducer,
                        Classifier* classifier,
                        ScaleSolution* scaleSolution = new ScaleSolution(1, 100.0),
                        FilterSolution* filterSolution = new FilterSolution(0.05, 0.95),
                        Configuration* conf = new Configuration()
                        );
        virtual ~EmotionDetector();
        void initialize(FacesImagesDatabase* database, Emotion basicEmotion);
        Emotion classify(cv::Mat basicExpression, cv::Mat specialExpression);
        float test(FacesImagesDatabase* testDatabase);
        float test(FacesImagesDatabase* testDatabase, std::string id);
    protected:
    private:
        cv::Mat countDifference(cv::Mat basicFaceExpression, cv::Mat specialFaceExpression);
        FacesDifferencesDatabase* prepareDiffs(FacesImagesDatabase* database, Emotion basicEmotion);
        ModelRegistrator* registrator;
        Classifier* classifier;
        ScaleSolution* scaleSolution;
        FilterSolution* filterSolution;
        FacesDifferencesDatabase* diffs;
        Configuration* conf;
        DimentionalityReducer* reducer;
};

#endif // EMOTIONDETECTOR_H
