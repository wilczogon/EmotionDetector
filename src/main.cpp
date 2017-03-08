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
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/ml.hpp>
#include <opencv/highgui.h>
#include <KnnClassifier.h>
#include <Translator.h>
#include <Configuration.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>

using namespace cv;

std::string HAPPY = "happy",
            NEUTRAL = "neutral",
            SAD = "sad",
            SURPRISED = "surprised",
            ANGRY = "angry";

std::list<std::string> listOfEmotions = {HAPPY, NEUTRAL, SAD, ANGRY};

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

FacesImagesDatabase* load(FacialLandmarkDetector* fdet,std::string metadataPath,std::string dataPath, bool heading, bool depthAdded, bool translateDepth, Configuration* conf){
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
        std::cout << Translator::toShort(record[2]) << "... " << std::endl;
        cv::Mat image = readImage(filePath.c_str());

        std::list<cv::Mat> facesPoints = fdet->getFacesPoints(image);

        if(facesPoints.size() >= 1){
            cv::Mat facePoints = *facesPoints.begin();
            cv::Mat completeFacePoints;

            if(depthAdded) {
                cv::Mat depth = readImage((dataPath + "/" + record[3]).c_str());
                cv::Mat z = cv::Mat(facePoints.rows, 1, CV_16S);
                completeFacePoints = cv::Mat(facePoints.rows, 3, CV_32F);
                for(int i = 0; i<facePoints.rows; ++i){
                    int depthValue = depth.at<short>(floor(facePoints.at<float>(i, 1)), floor(facePoints.at<float>(i, 0)));
                    completeFacePoints.at<float>(i, 0) = facePoints.at<float>(i, 1);
                    completeFacePoints.at<float>(i, 1) = facePoints.at<float>(i, 0);
                    completeFacePoints.at<float>(i, 2) = depthValue; // > 0 ? depthValue : 5000;
                }

                if(translateDepth)
                    Translator::translateCoords(completeFacePoints);
            } else {
                completeFacePoints = facePoints;
            }

            std::ofstream* file = conf->open("FacePoints");
            if(depthAdded)
                *file << "x,y,z" << std::endl;
            else
                *file << "x,y" << std::endl;
            for(int i = 0; i<completeFacePoints.rows; ++i) {
                for(int j = 0; j<completeFacePoints.cols; ++j) {
                    *file << completeFacePoints.at<float>(i, j);
                    if(j != completeFacePoints.cols-1)
                        *file << ",";
                }
                *file << std::endl;
            }
            delete file;

            /*file = conf->open("Image");
            *file << "x,y,z" << std::endl;
            for(int i = 0; i<480; i += 480/100)
                for(int j = 0; j<640; j += 640/100) {
                    int depthValue = depth.at<short>(i, j);
                    *file << i << "," << j << "," << (depthValue > 0 ? depthValue : 5000) << std::endl;
                }
            delete file;*/

            emotionPersonDataMap->add(filePath, record[0], Translator::toEmotion(record[1]), completeFacePoints);
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
        Configuration* conf = new Configuration("C:\\Users\\Mary\\Documents\\projekty\\na_uczelnie\\EmotionDetector\\visualization_data");

        bool translateDepth = true;
        bool depthAdded = true;
        FacesImagesDatabase* database = load(fdet, "C:\\Users\\Mary\\Documents\\dane\\obrazy\\faces\\metadata_training.csv", "C:\\Users\\Mary\\Documents\\dane\\obrazy\\faces\\attachments", true, depthAdded, translateDepth, conf);
        FacesImagesDatabase* testDatabase = load(fdet, "C:\\Users\\Mary\\Documents\\dane\\obrazy\\faces\\metadata_test.csv", "C:\\Users\\Mary\\Documents\\dane\\obrazy\\faces\\attachments", true, depthAdded, translateDepth, conf);

        std::cout << "\nAll images successfully loaded.\n";

        std::ofstream* file = conf->open("AdaBoost");

        float min = 1;
        int id = 0;
        int minId = 0;
        char str[20];
        float adaBoostBestRes;
        int adaBoostBestResIndex;
        float knnBestRes;
        int knnBestResIndex;
        *file << "weakCount, weightTrimRate, maxDepth, n, lowerBound, upperBound, result" << std::endl;
        for(int n = 1; n < 10; n++)
            for(int maxDepth = 1; maxDepth <= 3; ++maxDepth){
                std::cout << "tik tok" << std::endl;
                float weightTrimRate = 0.0;
                while(weightTrimRate < 1){
                    for(int weakCount = 1000; weakCount <= 5000; weakCount+=1000){
                        float lowerBound = 0.0;
                        while(lowerBound < 0.2){
                            lowerBound += 0.05;
                            float upperBound = 1.0;
                            while(upperBound > 0.8){
                                upperBound -= 0.05;
                                try {
                                    EmotionDetector* detector = new EmotionDetector(new ICPModelRegistrator(),
                                                                        new PCADimentionalityReducer(n),
                                                                        new AdaBoostClassifier(cv::ml::Boost::REAL, weakCount, weightTrimRate, maxDepth, false),
                                                                        new ScaleSolution(1, 100.0),
                                                                        new FilterSolution(lowerBound, upperBound),
                                                                        conf);
                                    detector->initialize(database, Emotion::neutral);
                                    itoa(id, str, 10);
                                    float res = detector->test(testDatabase, "AdaBoost_" + std::string(str));
                                    if(res < min) {
                                        min = res;
                                        minId = id;
                                    }
                                    //std::cout << weakCount << ", " << weightTrimRate << ", " << maxDepth << ", " << n << ", " << res << std::endl;
                                    *file << weakCount << ", " << weightTrimRate << ", " << maxDepth << ", " << n << ", " << lowerBound << ", " << upperBound << ", " << res << std::endl;
                                    delete detector;
                                    ++id;
                                } catch (...) {
                                    std::cout << "Error occured";
                                }
                            }
                        }
                    }
                    weightTrimRate+=0.2;
                }
            }

        delete file;

        adaBoostBestRes = min;
        adaBoostBestResIndex = minId;
        std::cout << "Ada Boost best result: " << min << " for id = " << minId << std::endl;

        min = 1;
        id = 0;
        minId = 0;

        file = conf->open("Knn");

        *file << "lowerThreshold, upperThreshold, n, k, result" << std::endl;
        float lowerThreshold = 0.0;
        while(lowerThreshold < 0.4){
            float upperThreshold = 1.0;
            while(upperThreshold > 0.6){
                std::cout << "tik tok" << std::endl;
                for(int k = 1; k<10; k+=1)
                    for(int n = 2; n<10; n+=1){
                        itoa(id, str, 10);
                        EmotionDetector* detector = new EmotionDetector(new ICPModelRegistrator(),
                                                                        new PCADimentionalityReducer(n),
                                                                        new KnnClassifier(k, conf, "Knn_" + std::string(str)),
                                                                        new ScaleSolution(1, 100.0),
                                                                        new FilterSolution(lowerThreshold, upperThreshold),
                                                                        conf);
                        detector->initialize(database, Emotion::neutral);
                        float res = detector->test(testDatabase, "Knn_" + std::string(str));
                        if(res < min) {
                            min = res;
                            minId = id;
                        }
                        *file << lowerThreshold << ", " << upperThreshold << ", " << n << ", " << k << ", " << res << std::endl;
                        delete detector;
                        ++id;
                    }

                upperThreshold -= 0.05;
            }
            lowerThreshold += 0.05;
        }

        delete file;

        knnBestRes = min;
        knnBestResIndex = minId;
        std::cout << "Knn best result: " << min << " for id = " << minId << std::endl;

        file = conf->open("BestResult");
        *file << "label, min, minId" << std::endl;
        *file << "Knn, " << knnBestRes << ", " << knnBestResIndex << std::endl;
        *file << "AdaBoost, " << adaBoostBestRes << ", " << adaBoostBestResIndex << std::endl;
        delete file;

        file = conf->open("Settings");
        *file << "depthAdded, translateDepth" << std::endl;
        *file << depthAdded << ", " << translateDepth << std::endl;
        delete file;

        return 0;
    } catch(const char* msg){
        std::cout << "Exception occurred: " << msg << std::endl;
    }
}
