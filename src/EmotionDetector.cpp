#include "EmotionDetector.h"
#include "Visualizer.h"
#include "FacialLandmarkDetector.h"
#include "ModelRegistrator.h"
#include <iostream>
#include <opencv2/core.hpp>

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

cv::Mat EmotionDetector::countDifference(cv::Mat basicFaceExpression, cv::Mat specialFaceExpression){
    cv::Mat result;
    cv::Mat registeredFaceExpression = registrator->registerModel(basicFaceExpression, specialFaceExpression);
    cv::subtract(registeredFaceExpression, basicFaceExpression, result);
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

float EmotionDetector::test(FacesImagesDatabase* testDatabase, bool loggingOn){
    int correctlyClassified = 0;
    int incorrectlyClassified = 0;

    FacesDifferencesDatabase* testDiffs = prepareDiffs(testDatabase, diffs->getBasicEmotion());

    for(auto emotion: testDiffs->getEmotions())
        for(int rowNo = 0; rowNo < testDiffs->get(emotion).rows; ++rowNo){
            cv::Mat diff = testDiffs->get(emotion).row(rowNo);
            if(classifier->classify(diff) == emotion)
                correctlyClassified++;
            else
                incorrectlyClassified++;
        }

    float result = incorrectlyClassified/float(correctlyClassified + incorrectlyClassified);

    if(loggingOn)
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
                    diffs->add(emotion, countDifference(basicData, specialData));
            /*for(int specialDataRowNo = 0; specialDataRowNo < database->get(personId, emotion).rows; ++specialDataRowNo){
                cv::Mat specialData = database->get(personId, emotion).row(specialDataRowNo);
                for(int basicDataRowNo = 0; basicDataRowNo < database->get(personId, basicEmotion).rows; ++basicDataRowNo){
                    cv::Mat basicData = database->get(personId, basicEmotion).row(basicDataRowNo);
                    diffs->add(emotion, countDifference(basicData, specialData));
                }
            }*/

    return diffs;
}
