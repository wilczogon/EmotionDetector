#include "EmotionDetector.h"
#include "ModelRegistrator.h"
#include <Translator.h>
#include <iostream>
#include <opencv2/core.hpp>

using namespace std;

EmotionDetector::EmotionDetector(ModelRegistrator* registrator,
                                 DimentionalityReducer* reducer,
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
    this->reducer = reducer;
}

EmotionDetector::~EmotionDetector()
{
    delete registrator;
    delete classifier;
    delete scaleSolution;
    delete filterSolution;
    delete reducer;
}

cv::Mat EmotionDetector::countDifference(cv::Mat basicFaceExpression, cv::Mat specialFaceExpression){
    cv::Mat result;
    cv::Mat registeredFaceExpression = registrator->registerModel(basicFaceExpression, specialFaceExpression);
    scaleSolution->scale(registeredFaceExpression, basicFaceExpression);
    cv::subtract(registeredFaceExpression, basicFaceExpression, result);
    filterSolution->filter(result);
    //TODO colors?
    return result.reshape(0, 1);
    registeredFaceExpression.release();
}

void EmotionDetector::initialize(FacesImagesDatabase* database, Emotion basicEmotion){
    diffs = prepareDiffs(database, basicEmotion);
    cv::Mat samples;
    cv::Mat responses;
    std::vector<std::string> names;
    diffs->getData(names, samples, responses);
    reducer->initialize(samples);
    classifier->initialize(reducer->reduceDimentionality(samples), responses);
    samples.release();
    responses.release();
}

Emotion EmotionDetector::classify(cv::Mat basicExpression, cv::Mat specialExpression){
    return classifier->classify(reducer->reduceDimentionality(countDifference(basicExpression, specialExpression)));
}

float EmotionDetector::test(FacesImagesDatabase* testDatabase, std::string id){
    int correctlyClassified = 0;
    int incorrectlyClassified = 0;

    FacesDifferencesDatabase* testDiffs = prepareDiffs(testDatabase, diffs->getBasicEmotion());

    std::ofstream* file = conf->open("TestResult_" + id);
    *file << "actual emotion, recognized emotion" << std::endl;

    for(auto emotion: testDiffs->getEmotions()){
        if(emotion == diffs->getBasicEmotion())
            continue;
        std::vector<std::string> names = testDiffs->getNames(emotion);
        for(int rowNo = 0; rowNo < testDiffs->get(emotion).rows; ++rowNo){
            cv::Mat diff = testDiffs->get(emotion).row(rowNo);

            int classifiedEmotion = classifier->classify(reducer->reduceDimentionality(diff));

            *file << emotion << ", " << classifiedEmotion << std::endl;

            if(classifiedEmotion == emotion)
                correctlyClassified++;
            else
                incorrectlyClassified++;

            diff.release();
        }
    }

    delete testDiffs;
    delete file;

    float result = incorrectlyClassified/float(correctlyClassified + incorrectlyClassified);

    if(conf->isTalkative())
        std::cout << "Correctly classified elements: " << correctlyClassified << std::endl << "Incorrectly classified elements: " << incorrectlyClassified << std::endl << "Error: " << result*100 << "%" << std::endl;

    return result;
}

float EmotionDetector::test(FacesImagesDatabase* testDatabase){
    test(testDatabase, "");
}

FacesDifferencesDatabase* EmotionDetector::prepareDiffs(FacesImagesDatabase* database, Emotion basicEmotion){
    if(database->getEmotions().size() == 0)
        throw "Empty data set.";

    if(database->getEmotions().find(basicEmotion) == database->getEmotions().end())
        throw "Empty set of basic expressions.";

    cv::Mat tmp;

    for(auto personId: database->getPersonIds())
        for(auto emotion: database->getEmotions())
            for(auto specialData: database->get(personId, emotion))
                for(auto basicData: database->get(personId, basicEmotion))
                    tmp.push_back(countDifference(basicData.second, specialData.second));

    filterSolution->initialize(tmp);
    FacesDifferencesDatabase* diffs = new FacesDifferencesDatabase(basicEmotion);

    for(auto personId: database->getPersonIds())
        for(auto emotion: database->getEmotions())
            for(auto specialData: database->get(personId, emotion))
                for(auto basicData: database->get(personId, basicEmotion))
                    diffs->add(Translator::toShort(basicData.first) + " - " + Translator::toShort(specialData.first), emotion, countDifference(basicData.second, specialData.second));

    return diffs;
}
