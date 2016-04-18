#include "EmotionDetector.h"
#include "Visualizer.h"
#include "FileStream.h"
#include "FacialLandmarkDetector.h"
#include "DlibFacialLandmarkDetector.h"
#include "ModelRegistrator.h"
#include "SimpleModelRegistrator.h"
#include "DimentionalityReducer.h"
#include "PCADimentionalityReducer.h"
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <dlib/image_processing.h>
#include <dlib/image_io.h>

using namespace dlib;
using namespace std;

EmotionDetector::EmotionDetector(ModelRegistrator* registrator, Classifier* classifier)
{
    this->registrator = registrator;
    this->classifier = classifier;
}

EmotionDetector::~EmotionDetector()
{
    //dtor
}

std::vector<float> EmotionDetector::countDifference(std::list<std::vector<float> > basicFaceExpression, std::list<std::vector<float> > specialFaceExpression){

    if(basicFaceExpression.size() != specialFaceExpression.size()){
        throw "Not equal number of points";
    }

    std::vector<float> result;
    std::list<std::vector<float> >::iterator basicIt;
    std::list<std::vector<float> >::iterator expIt;

    for(basicIt = basicFaceExpression.begin(), expIt = specialFaceExpression.begin();
        basicIt != basicFaceExpression.end(), expIt != specialFaceExpression.end();
        ++basicIt, ++expIt){

            if((*basicIt).size()!=(*expIt).size())
                throw "Points with different numbers of coordinates";

            if((*basicIt).size() < 1 || (*expIt).size() < 1)
                throw "Zero size points.";
    }

    basicFaceExpression = registrator->registerModel(specialFaceExpression, basicFaceExpression);

    for(basicIt = basicFaceExpression.begin(), expIt = specialFaceExpression.begin();
        basicIt != basicFaceExpression.end() || expIt != specialFaceExpression.end();
        ++basicIt, ++expIt){

            for(unsigned int i = 0; i<(*basicIt).size(); ++i){
                result.push_back((*expIt)[i] - (*basicIt)[i]);
            }
    }

    //TODO colors?

    return result;

}

/*Space* EmotionDetector::createSpaceOfDifferences(std::list<std::vector<float> > differences, std::list<Emotion> emotions){
    if(differences.size() != emotions.size())
        throw "Not equal number of differences and emotions";

    reducer->initialize(differences);

    std::list<void*> data;
    std::list<Emotion>::iterator iter;
    for(iter = emotions.begin(); iter != emotions.end(); ++iter){
        int* emotionInt = new int[1];
        emotionInt[0] = (*iter);
        data.push_back(emotionInt);
    }

    return new Space(reducer->reduceDimentionality(differences), data);
}*/

void EmotionDetector::initialize(FacesImagesDatabase* database, Emotion basicEmotion){
    diffs = prepareDiffs(database, basicEmotion);
}

Emotion EmotionDetector::classify(std::list<std::vector<float> > basicExpression, std::list<std::vector<float> > specialExpression){
    return classifier->classify(countDifference(basicExpression, specialExpression));
}

float EmotionDetector::test(FacesImagesDatabase* testDatabase, bool loggingOn){
    int correctlyClassified = 0;
    int incorrectlyClassified = 0;

    FacesDifferencesDatabase* testDiffs = prepareDiffs(testDatabase, diffs->getBasicEmotion());

    for(auto emotion: testDiffs->getEmotions())
        for(auto diff: testDiffs->get(emotion))
            if(classifier->classify(diff) == emotion)
                correctlyClassified++;
            else
                incorrectlyClassified++;

    float result = incorrectlyClassified/float(correctlyClassified + incorrectlyClassified);

    if(loggingOn)
        std::cout << "Correctly classified elements: " << correctlyClassified << std::endl << "Incorrectly classified elements: " << incorrectlyClassified << std::endl << "Error: " << result << std::endl;

    return result;
}

FacesDifferencesDatabase* EmotionDetector::prepareDiffs(FacesImagesDatabase* database, Emotion basicEmotion){
    FacesDifferencesDatabase* diffs = new FacesDifferencesDatabase(basicEmotion);

    if(database->getEmotions().size() == 0)
        throw "Empty data set.";

    if(database->getEmotions().find(basicEmotion) == database->getEmotions().end())
        throw "Empty set of basic expressions.";

    for(auto personId: database->getPersonIds())
        for(auto emotion: database->getEmotions())
            for(auto specialData: database->get(personId, emotion))
                for(auto basicData: database->get(personId, basicEmotion))
                    diffs->add(emotion, countDifference(basicData, specialData));

    return diffs;
}
