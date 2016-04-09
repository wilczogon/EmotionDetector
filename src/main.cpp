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

std::map<std::string, std::map<std::string, std::list<std::list<std::vector<float> > > > > load(FacialLandmarkDetector* fdet,std::string path, std::list<std::string> emotionIds, std::list<std::string> personIds, std::string separator = " ", std::string extension = "png"){
    std::map<std::string, std::map<std::string, std::list<std::list<std::vector<float> > > > > emotionPersonDataMap;

    for(std::string &emotionId: emotionIds){
        emotionPersonDataMap[emotionId] = std::map<std::string, std::list<std::list<std::vector<float> > > >();
        for(std::string &personId: personIds){
            emotionPersonDataMap[emotionId][personId] = std::list<std::list<std::vector<float> > >();

            std::string filePath = path + "/" + emotionId + separator + personId + "." + extension;
            if(fileExists(filePath)){
                std::cout << "Processing: " << filePath << std::endl;
                std::list<std::list<std::vector<float> > > tmp = fdet->getFacesPoints(readImage(filePath.c_str()));
                if(tmp.size() >= 1){
                    std::cout << "Face points retrieved. (" << tmp.size() << " x " << (*tmp.begin()).size() << ")" << std::endl;
                    emotionPersonDataMap[emotionId][personId].push_back((*tmp.begin()));
                }
            }

            for(int i = 1;; ++i){
                filePath = path + "/" + emotionId + separator + personId + " (" + std::to_string(i) + ")." + extension;
                if(!fileExists(filePath))
                    break;
                std::cout << "Processing: " << filePath << std::endl;
                std::list<std::list<std::vector<float> > > tmp = fdet->getFacesPoints(readImage(filePath.c_str()));
                if(tmp.size() >= 1){
                    std::cout << "Face points retrieved. (" << tmp.size() << " x " << (*tmp.begin()).size() << ")" << std::endl;
                    emotionPersonDataMap[emotionId][personId].push_back((*tmp.begin()));
                }
            }
        }
    }

    return emotionPersonDataMap;
}

std::list<std::vector<float> > countDifference(std::map<std::string, std::map<std::string, std::list<std::list<std::vector<float> > > > > data,
                EmotionDetector* detector,
                std::string basicExpression,
                std::string specialExpression){

    std::list<std::vector<float> > result;

    if(data.size() == 0)
        throw "Empty data set.";

    if(data[basicExpression].size() == 0)
        throw "Empty set of basic expressions.";

    if(data[specialExpression].size() == 0)
        throw "Empty set of non-basic expressions.";

    std::map<std::string, std::list<std::list<std::vector<float> > > >::iterator it;
    for(it = data[basicExpression].begin(); it != data[basicExpression].end(); it++){
        std::string personId = it->first;
        for(auto basicExpressionForPersonId: data[basicExpression][personId]){
            for(auto specialExpressionForPersonId: data[specialExpression][personId]){
                result.push_back(detector->countDifference(basicExpressionForPersonId, specialExpressionForPersonId));
            }
        }
    }

    return result;
}

int main(){
    try{
        FacialLandmarkDetector* fdet = new DlibFacialLandmarkDetector();
        EmotionDetector* detector = new EmotionDetector(new ICPModelRegistrator(), new PCADimentionalityReducer());

        std::cout << "\nAll classes successfully initialized.\n";

        std::list<std::string> personIds = {"a", "b", "c", "d", "e", "f", "g"};

        std::map<std::string, std::map<std::string, std::list<std::list<std::vector<float> > > > > data = load(fdet, "C:\\Users\\Mary\\Desktop\\attachments\\colour", listOfEmotions, personIds, "_", "png");

        std::cout << "\nAll images successfully loaded.\n";

        std::list<std::vector<float> > diffs;
        std::list<Emotion> emotions;

        for(auto emotionName: listOfEmotions){
            std::list<std::vector<float> > tmp = countDifference(data, detector, SAD, emotionName);

            for(int i = 0; i<tmp.size(); ++i){
                emotions.push_back(toEmotion(emotionName));
            }

            diffs.splice(diffs.end(), tmp);
        }

        Visualizer* visualizer = new Visualizer();
        Space* space = detector->createSpaceOfDifferences(diffs, emotions);
        visualizer->visualize("Points", space, &emotionToColor, 640, 480);

        data.clear();

        return 0;
    } catch(const char* msg){
        std::cout << "Exception occurred: " << msg << std::endl;
    }
}
