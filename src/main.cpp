#include <iostream>
#include <list>
#include <map>
#include <vector>
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
#include <png.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv/highgui.h>
#include <ReduceKnnClassifier.h>
#include <Translator.h>
#include <Configuration.h>

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

FacesImagesDatabase* load(FacialLandmarkDetector* fdet,std::string metadataPath,std::string dataPath, bool heading){
    FacesImagesDatabase* emotionPersonDataMap = new FacesImagesDatabase();

    std::cout << "Metadata path: " << metadataPath << std::endl;
    std::cout << "Data path: " << dataPath << std::endl;
    std::cout << "Processing images:" << std::endl;

    ifstream infile(metadataPath);

    bool first = true;
    while (infile)
    {
        std::string s;
        if (!getline( infile, s )) break;

        if(heading && first){
            first = false;
            continue;
        }

        istringstream ss( s );
        std::vector <std::string> record;

        while (ss)
        {
            string s;
            if (!getline( ss, s, ';' )) break;
            record.push_back( s );
        }

        std::string filePath = dataPath + "/" + record[2];
        std::cout << Translator::toShort(record[2]) << "... ";
        std::list<cv::Mat> tmp = fdet->getFacesPoints(readImage(filePath.c_str()));
        if(tmp.size() >= 1){
            std::cout << tmp.size() << " x " << (*tmp.begin()).size() << std::endl;
            emotionPersonDataMap->add(filePath, record[0], Translator::toEmotion(record[1]), *tmp.begin());
        }
    }
    if (!infile.eof())
    {
        cerr << "Error during loading data\n";
    }

    return emotionPersonDataMap;
}

int main(){
    try{
        FacialLandmarkDetector* fdet = new DlibFacialLandmarkDetector();
        Configuration* conf = new Configuration("C:\\Users\\Mary\\Documents\\Mary\\9_semestr\\magisterka\\project\\visualization_data");

        FacesImagesDatabase* database = load(fdet, "C:\\Users\\Mary\\Documents\\Mary\\9_semestr\\magisterka\\project\\metadata_training.csv", "C:\\Users\\Mary\\Desktop\\attachments", true);
        FacesImagesDatabase* testDatabase = load(fdet, "C:\\Users\\Mary\\Documents\\Mary\\9_semestr\\magisterka\\project\\metadata_test.csv", "C:\\Users\\Mary\\Desktop\\attachments", true);

        std::cout << "\nAll images successfully loaded.\n";

        /*std::cout << "AdaBoost" << std::endl << "weakCount, weightTrimRate, maxDepth, useSurrogates, result" << std::endl;
        for(bool useSurrogates: {false})
            for(int maxDepth = 1; maxDepth < 50; maxDepth += 5){
                float weightTrimRate = 0.1;
                while(weightTrimRate < 1.0){
                    for(int weakCount = 10; weakCount < 100000; weakCount*=10){
                        EmotionDetector* detector = new EmotionDetector(new ICPModelRegistrator(),
                                                                        new AdaBoostClassifier(weakCount, weightTrimRate, maxDepth, useSurrogates),
                                                                        new ScaleSolution(1, 100.0),
                                                                        new FilterSolution(0.0, 2.0),
                                                                        conf);
                        detector->initialize(database, Emotion::sad);
                        float res = detector->test(testDatabase);
                        std::cout << weakCount << ", " << weightTrimRate << ", " << maxDepth << ", " << useSurrogates << ", " << res << std::endl;
                    }
                    weightTrimRate+=0.1;
                }
            }*/

        std::cout << "Reduce Knn" << std::endl << "lowerThreshold, upperThreshold, n, k, result" << std::endl;
        float lowerThreshold = 0.0;
        float upperThreshold = 0.9;
        while(lowerThreshold <= 0.1){
            while(upperThreshold <= 1.0){
                for(int k = 1; k<5; ++k)
                    for(int n = 2; n<6; ++n){
                        EmotionDetector* detector = new EmotionDetector(new ICPModelRegistrator(),
                                                                        new ReduceKnnClassifier(new PCADimentionalityReducer(n), k, conf),
                                                                        new ScaleSolution(1, 100.0),
                                                                        new FilterSolution(lowerThreshold, upperThreshold),
                                                                        conf);
                        detector->initialize(database, Emotion::sad);
                        float res = detector->test(testDatabase);
                        std::cout << lowerThreshold << ", " << upperThreshold << ", " << n << ", " << k << ", " << res << std::endl;
                    }

                upperThreshold += 0.01;
            }
            lowerThreshold += 0.01;
        }

        return 0;
    } catch(const char* msg){
        std::cout << "Exception occurred: " << msg << std::endl;
    }
}
