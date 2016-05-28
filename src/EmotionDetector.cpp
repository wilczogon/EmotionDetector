#include "EmotionDetector.h"
#include "Visualizer.h"
#include "FacialLandmarkDetector.h"
#include "ModelRegistrator.h"
#include <Translator.h>
#include <iostream>
#include <opencv2/core.hpp>

using namespace std;

EmotionDetector::EmotionDetector(ModelRegistrator* registrator,
                                 Classifier* classifier,
                                 ScaleSolution* scaleSolution,
                                 FilterSolution* filterSolution,
                                 Configuration* conf)
{
    this->registrator = registrator;
    this->classifier = classifier;
    this->scaleSolution = scaleSolution;
    this->filterSolution = filterSolution;
    this->conf = conf;
}

EmotionDetector::~EmotionDetector()
{
    //dtor
}

cv::Mat EmotionDetector::countDifference(cv::Mat basicFaceExpression, cv::Mat specialFaceExpression){
    cv::Mat result;
    cv::Mat registeredFaceExpression = registrator->registerModel(basicFaceExpression, specialFaceExpression);
    scaleSolution->scale(registeredFaceExpression, basicFaceExpression);
    cv::subtract(registeredFaceExpression, basicFaceExpression, result);
    filterSolution->filter(result);
    //TODO colors?
    return result.reshape(0, 1);
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
    classifier->initialize(diffs);
}

Emotion EmotionDetector::classify(cv::Mat basicExpression, cv::Mat specialExpression){
    return classifier->classify(countDifference(basicExpression, specialExpression));
}

float EmotionDetector::test(FacesImagesDatabase* testDatabase){
    int correctlyClassified = 0;
    int incorrectlyClassified = 0;

    FacesDifferencesDatabase* testDiffs = prepareDiffs(testDatabase, diffs->getBasicEmotion());

    for(auto emotion: testDiffs->getEmotions()){
        std::vector<std::string> names = testDiffs->getNames(emotion);
        for(int rowNo = 0; rowNo < testDiffs->get(emotion).rows; ++rowNo){
            cv::Mat diff = testDiffs->get(emotion).row(rowNo);
            //
            //std::cout << names[rowNo] << "\n" << "Actual emotion:\t" << Translator::toString(emotion) << "\nClassified as:\t" << Translator::toString(classifier->classify(diff)) << std::endl << std::endl;
            if(classifier->classify(diff) == emotion)
                correctlyClassified++;
            else
                incorrectlyClassified++;
        }
    }

    float result = incorrectlyClassified/float(correctlyClassified + incorrectlyClassified);

    if(conf->isTalkative())
        std::cout << "Correctly classified elements: " << correctlyClassified << std::endl << "Incorrectly classified elements: " << incorrectlyClassified << std::endl << "Error: " << result*100 << "%" << std::endl;

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
                    diffs->add(Translator::toShort(basicData.first) + " - " + Translator::toShort(specialData.first), emotion, countDifference(basicData.second, specialData.second));

    return diffs;
}
