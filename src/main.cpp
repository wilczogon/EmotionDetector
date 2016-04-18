#include <iostream>
#include <list>
#include <map>
#include <FacialLandmarkDetector.h>
#include <DlibFacialLandmarkDetector.h>
#include <EmotionDetector.h>
#include <ModelRegistrator.h>
#include <ICPModelRegistrator.h>
#include <DimentionalityReducer.h>
#include <PCADimentionalityReducer.h>
#include <FacesDifferencesDatabase.h>
#include <FacesImagesDatabase.h>
#include <AdaBoostClassifier.h>
#include <sys/stat.h>
#include <Visualizer.h>
#include <SFML/Graphics.hpp>
#include <png.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv/highgui.h>

using namespace cv;

std::string HAPPY = "happy",
            NEUTRAL = "neutral",
            SAD = "sad",
            SURPRISED = "surprised",
            ANGRY = "angry";

std::list<std::string> listOfEmotions = {HAPPY, NEUTRAL, SAD, SURPRISED, ANGRY};

sf::Color emotionToColor(void* classPoint){
    Emotion emotion = (Emotion)(*((int*)classPoint));

    if(emotion == Emotion::happy)
        return sf::Color::Green;
    else if(emotion == Emotion::neutral)
        return sf::Color::Yellow;
    else if(emotion == Emotion::sad)
        return sf::Color::Blue;
    else if(emotion == Emotion::surprised)
        return sf::Color::Magenta;
    else if(emotion == Emotion::angry)
        return sf::Color::Red;
    else
        return sf::Color(10, 10, 10, 255);
}

Emotion toEmotion(std::string name){
    if(name == HAPPY)
        return happy;
    else if(name == NEUTRAL)
        return neutral;
    else if(name == SAD)
        return sad;
    else if(name == SURPRISED)
        return surprised;
    else if(name == ANGRY)
        return angry;
    else
        return (Emotion)NULL;
}

std::string toString(Emotion emotion){
    if(emotion == Emotion::happy)
        return HAPPY;
    else if(emotion == Emotion::neutral)
        return NEUTRAL;
    else if(emotion == Emotion::sad)
        return SAD;
    else if(emotion == Emotion::surprised)
        return SURPRISED;
    else if(emotion == Emotion::angry)
        return ANGRY;
    else
        return (std::string)NULL;
}

bool fileExists(std::string fileName){
    struct stat buf;
    if (stat(fileName.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}

Mat readImage(const char* imageFile){
    return imread(String(imageFile), CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
}

FacesImagesDatabase* load(FacialLandmarkDetector* fdet,std::string path, std::list<std::string> emotionIds, std::list<std::string> personIds, std::string separator = " ", std::string extension = "png"){
    FacesImagesDatabase* emotionPersonDataMap = new FacesImagesDatabase();

    for(std::string &emotionId: emotionIds){
        for(std::string &personId: personIds){

            std::string filePath = path + "/" + emotionId + separator + personId + "." + extension;
            if(fileExists(filePath)){
                std::cout << "Processing: " << filePath << std::endl;
                std::list<cv::Mat> tmp = fdet->getFacesPoints(readImage(filePath.c_str()));
                if(tmp.size() >= 1){
                    std::cout << "Face points retrieved. (" << tmp.size() << " x " << (*tmp.begin()).size() << ")" << std::endl;
                    emotionPersonDataMap->add(personId, toEmotion(emotionId), *tmp.begin());
                }
            }

            for(int i = 1;; ++i){
                filePath = path + "/" + emotionId + separator + personId + " (" + std::to_string(i) + ")." + extension;
                if(!fileExists(filePath))
                    break;
                std::cout << "Processing: " << filePath << std::endl;
                std::list<cv::Mat> tmp = fdet->getFacesPoints(readImage(filePath.c_str()));
                if(tmp.size() >= 1){
                    std::cout << "Face points retrieved. (" << tmp.size() << " x " << (*tmp.begin()).size() << ")" << std::endl;
                    emotionPersonDataMap->add(personId, toEmotion(emotionId), *tmp.begin());
                }
            }
        }
    }

    return emotionPersonDataMap;
}

int main(){
    try{
        FacialLandmarkDetector* fdet = new DlibFacialLandmarkDetector();
        EmotionDetector* detector = new EmotionDetector(new ICPModelRegistrator(), new AdaBoostClassifier());

        std::cout << "\nAll classes successfully initialized.\n";

        std::list<std::string> personIds = {"a", "b", "c", "d", "e", "f", "g"};
        std::list<std::string> testPersonIds = {"gibbs", "jack_black", "gandalf"};

        //FacesImagesDatabase* database = load(fdet, "C:\\Users\\Mary\\Desktop\\attachments\\colour", listOfEmotions, personIds, "_", "png");
        FacesImagesDatabase* testDatabase = load(fdet, "C:\\Users\\Mary\\Documents\\Mary\\9_semestr\\magisterka\\project\\test_images", listOfEmotions, testPersonIds, "_", "jpg");
        FacesImagesDatabase* database = testDatabase;

        std::cout << "\nAll images successfully loaded.\n";

        detector->initialize(database, Emotion::sad);
        detector->test(testDatabase, true);

        return 0;
    } catch(const char* msg){
        std::cout << "Exception occurred: " << msg << std::endl;
    }
}
