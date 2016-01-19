#include <iostream>
#include <list>
#include <map>
#include <FacialLandmarkDetector.h>
#include <DlibFacialLandmarkDetector.h>
#include <EmotionDetector.h>
#include <sys/stat.h>

std::string SMILING = "smiling",
            NEUTRAL = "neutral",
            SAD = "sad",
            SURPRISED = "surprised",
            ANGRY = "angry";

bool fileExists(std::string fileName){
    struct stat buf;
    if (stat(fileName.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
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
                std::list<std::list<std::vector<float> > > tmp = fdet->getFacesPoints(filePath);
                if(tmp.size() >= 1){
                    std::cout << "Face points retrieved." << std::endl;
                    emotionPersonDataMap[emotionId][personId].push_back((*tmp.begin()));
                }
            }

            for(int i = 1;; ++i){
                filePath = path + "/" + emotionId + separator + personId + " (" + std::to_string(i) + ")." + extension;
                if(!fileExists(filePath))
                    break;
                std::cout << "Processing: " << filePath << std::endl;
                std::list<std::list<std::vector<float> > > tmp = fdet->getFacesPoints(filePath);
                if(tmp.size() >= 1){
                    std::cout << "Face points retrieved." << std::endl;
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
    for(auto iter: data[basicExpression]){

    }
}

int main(){
    FacialLandmarkDetector* fdet = new DlibFacialLandmarkDetector();
    EmotionDetector* detector = new EmotionDetector();

    std::list<std::string> emotionIds;
    emotionIds.push_back("neutral");
    emotionIds.push_back("smiling");
    emotionIds.push_back("sad");
    emotionIds.push_back("angry");
    emotionIds.push_back("surprised");

    std::list<std::string> personIds;
    personIds.push_back("mp");

    std::map<std::string, std::map<std::string, std::list<std::list<std::vector<float> > > > > data = load(fdet, "C:\\Users\\Mary\\Pictures\\Picasa\\Exports\\Captured Videos", emotionIds, personIds, " ", "jpg");
    std::list<std::vector<float> > diffs = countDifference(data, detector, "neutral", "smiling");
}
