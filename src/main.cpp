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
#include <ReduceKnnClassifier.h>
#include <Translator.h>

using namespace cv;

std::string HAPPY = "happy",
            NEUTRAL = "neutral",
            SAD = "sad",
            SURPRISED = "surprised",
            ANGRY = "angry";

std::list<std::string> listOfEmotions = {HAPPY, NEUTRAL, SAD, SURPRISED, ANGRY};

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

    std::cout << "Processing images:" << std::endl;

    for(std::string &emotionId: emotionIds){
        for(std::string &personId: personIds){

            std::string filePath = path + "/" + emotionId + separator + personId + "." + extension;
            if(fileExists(filePath)){
                std::cout << Translator::toShort(emotionId + separator + personId + "." + extension) << "... ";
                std::list<cv::Mat> tmp = fdet->getFacesPoints(readImage(filePath.c_str()));
                if(tmp.size() >= 1){
                    std::cout << tmp.size() << " x " << (*tmp.begin()).size() << std::endl;
                    emotionPersonDataMap->add(filePath, personId, Translator::toEmotion(emotionId), *tmp.begin());
                }
            }

            for(int i = 1;; ++i){
                filePath = path + "/" + emotionId + separator + personId + " (" + std::to_string(i) + ")." + extension;
                if(!fileExists(filePath))
                    break;
                std::cout << Translator::toShort(emotionId + separator + personId + " (" + std::to_string(i) + ")." + extension) << "... ";
                std::list<cv::Mat> tmp = fdet->getFacesPoints(readImage(filePath.c_str()));
                if(tmp.size() >= 1){
                    std::cout << tmp.size() << " x " << (*tmp.begin()).size() << std::endl;
                    emotionPersonDataMap->add(filePath, personId, Translator::toEmotion(emotionId), *tmp.begin());
                }
            }
        }
    }

    return emotionPersonDataMap;
}

int main(){
    try{
        FacialLandmarkDetector* fdet = new DlibFacialLandmarkDetector();

        FacesImagesDatabase* database = load(fdet, "C:\\Users\\Mary\\Desktop\\attachments\\colour", listOfEmotions, {"d", "e", "f", "g"}, "_", "png");
        FacesImagesDatabase* testDatabase = load(fdet, "C:\\Users\\Mary\\Desktop\\attachments\\colour", listOfEmotions, {"a", "b", "c"}, "_", "png");
        //FacesImagesDatabase* testDatabase = load(fdet, "C:\\Users\\Mary\\Documents\\Mary\\9_semestr\\magisterka\\project\\test_images", listOfEmotions, {"gibbs", "jack_black", "gandalf"}, "_", "jpg");
        //FacesImagesDatabase* database = testDatabase;//load(fdet, "C:\\Users\\Mary\\Documents\\Mary\\9_semestr\\magisterka\\project\\test_images", listOfEmotions, {"gandalf"}, "_", "jpg");

        std::cout << "\nAll images successfully loaded.\n";

        /*std::cout << "AdaBoost" << std::endl << "weakCount, weightTrimRate, maxDepth, useSurrogates, result" << std::endl;
        for(bool useSurrogates: {false})
            for(int maxDepth = 1; maxDepth < 50; maxDepth += 5){
                float weightTrimRate = 0.1;
                while(weightTrimRate < 1.0){
                    for(int weakCount = 10; weakCount < 100000; weakCount*=10){
                        EmotionDetector* detector = new EmotionDetector(new ICPModelRegistrator(), new AdaBoostClassifier(weakCount, weightTrimRate, maxDepth, useSurrogates)); //new ReduceKnnClassifier(new PCADimentionalityReducer(5), 3));
                        detector->initialize(database, Emotion::sad);
                        float res = detector->test(testDatabase, false);
                        std::cout << weakCount << ", " << weightTrimRate << ", " << maxDepth << ", " << useSurrogates << ", " << res << std::endl;
                    }
                    weightTrimRate+=0.1;
                }
            }*/

        std::cout << "Reduce Knn" << std::endl << "n, k, result" << std::endl;
        for(int k = 1; k<10; ++k)
            for(int n = 2; n<5; ++n){
                EmotionDetector* detector = new EmotionDetector(new ICPModelRegistrator(), new ReduceKnnClassifier(new PCADimentionalityReducer(n), k));
                detector->initialize(database, Emotion::sad);
                float res = detector->test(testDatabase, false);
                std::cout << n << ", " << k << ", " << res << std::endl;
            }

        return 0;
    } catch(const char* msg){
        std::cout << "Exception occurred: " << msg << std::endl;
    }
}
