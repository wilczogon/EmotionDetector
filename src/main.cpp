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
        ModelRegistrator* registrator = new ICPModelRegistrator();
        DimentionalityReducer* reducer = new PCADimentionalityReducer();
        EmotionDetector* detector = new EmotionDetector(registrator, reducer);

        std::cout << "\nAll classes successfully initialized.\n";

        std::list<std::string> emotionIds;
        emotionIds.push_back("neutral");
        emotionIds.push_back("smiling");
        emotionIds.push_back("sad");
        emotionIds.push_back("angry");
        emotionIds.push_back("surprised");

        std::list<std::string> personIds;
        personIds.push_back("mp");

        std::map<std::string, std::map<std::string, std::list<std::list<std::vector<float> > > > > data = load(fdet, "C:\\Users\\Mary\\Pictures\\Picasa\\Exports\\Captured Videos", emotionIds, personIds, " ", "jpg");

        std::cout << "\nAll images successfully loaded.\n";

        std::list<std::vector<float> > diffs = countDifference(data, detector, "neutral", "smiling");

        data.clear();

        return 0;
    } catch(const char* msg){
        std::cout << "Exception occurred: " << msg << std::endl;
    }
}
