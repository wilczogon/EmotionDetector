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
#include <iostream>
#include <fstream>

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

FacesImagesDatabase* load(FacialLandmarkDetector* fdet,std::string metadataPath,std::string dataPath, bool heading, Configuration* conf){
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
        cv::Mat depth = readImage((dataPath + "/" + record[3]).c_str());

        std::list<cv::Mat> facesPoints = fdet->getFacesPoints(image);

        if(facesPoints.size() >= 1){
            cv::Mat facePoints = *facesPoints.begin();

            cv::Mat z = cv::Mat(facePoints.rows, 1, CV_32F);
            for(int i = 0; i<facePoints.rows; ++i){
                z.at<float>(i, 0) = (float)depth.at<unsigned short>((int)facePoints.at<float>(i, 0), (int)facePoints.at<float>(i, 1));
            }

            cv::hconcat(facePoints, z, facePoints);
            Translator::translateCoords(facePoints);

            std::ofstream file = conf->open("FacePoints");
            for(int i = 0; i<facePoints.rows; ++i) {
                for(int j = 0; j<facePoints.cols; ++j) {
                    file << facePoints.at<float>(i, j);
                    if(j != facePoints.cols-1)
                        file << ",";
                }
                file << std::endl;
            }
            file.close();

            emotionPersonDataMap->add(filePath, record[0], Translator::toEmotion(record[1]), facePoints);
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

        FacesImagesDatabase* database = load(fdet, "C:\\Users\\Mary\\Documents\\Mary\\9_semestr\\magisterka\\project\\metadata_training.csv", "C:\\Users\\Mary\\Desktop\\attachments", true, conf);
        FacesImagesDatabase* testDatabase = load(fdet, "C:\\Users\\Mary\\Documents\\Mary\\9_semestr\\magisterka\\project\\metadata_test.csv", "C:\\Users\\Mary\\Desktop\\attachments", true, conf);

        std::cout << "\nAll images successfully loaded.\n";

        std::ofstream file = conf->open("AdaBoost");

        std::cout << "AdaBoost" << std::endl << "weakCount, weightTrimRate, maxDepth, useSurrogates, n, result" << std::endl;
        file << "AdaBoost" << std::endl << "weakCount, weightTrimRate, maxDepth, useSurrogates, n, result" << std::endl;
        for(int n = 1; n < 8; n+=1)
            for(int maxDepth = 15; maxDepth < 20; maxDepth += 1){
                float weightTrimRate = 0.1;
                while(weightTrimRate < 1.0){
                    for(int weakCount = 100; weakCount < 100000; weakCount*=10){

                        EmotionDetector* detector = new EmotionDetector(new ICPModelRegistrator(),
                                                                        new PCADimentionalityReducer(n),
                                                                        new AdaBoostClassifier(weakCount, weightTrimRate, maxDepth, false),
                                                                        new ScaleSolution(1, 100.0),
                                                                        new FilterSolution(0.0, 2.0),
                                                                        conf);
                        detector->initialize(database, Emotion::neutral);
                        float res = detector->test(testDatabase);
                        std::cout << weakCount << ", " << weightTrimRate << ", " << maxDepth << ", " << n << ", " << res << std::endl;
                        file << weakCount << ", " << weightTrimRate << ", " << maxDepth << ", " << n << ", " << res << std::endl;
                        delete detector;
                    }
                    weightTrimRate+=0.1;
                }
            }

        file.close();

        file = conf->open("ReduceKnn");

        std::cout << "Reduce Knn" << std::endl << "lowerThreshold, upperThreshold, n, k, result" << std::endl;
        file << "Reduce Knn" << std::endl << "lowerThreshold, upperThreshold, n, k, result" << std::endl;
        float lowerThreshold = 0.0;
        float upperThreshold = 0.9;
        while(lowerThreshold <= 0.5){
            while(upperThreshold > 0.5){
                for(int k = 1; k<15; ++k)
                    for(int n = 2; n<20; ++n){
                        EmotionDetector* detector = new EmotionDetector(new ICPModelRegistrator(),
                                                                        new PCADimentionalityReducer(n),
                                                                        new ReduceKnnClassifier(k, conf),
                                                                        new ScaleSolution(1, 100.0),
                                                                        new FilterSolution(lowerThreshold, upperThreshold),
                                                                        conf);
                        detector->initialize(database, Emotion::neutral);
                        float res = detector->test(testDatabase);
                        std::cout << lowerThreshold << ", " << upperThreshold << ", " << n << ", " << k << ", " << res << std::endl;
                        file << lowerThreshold << ", " << upperThreshold << ", " << n << ", " << k << ", " << res << std::endl;
                        delete detector;
                    }

                upperThreshold -= 0.01;
            }
            lowerThreshold += 0.01;
        }

        file.close();

        return 0;
    } catch(const char* msg){
        std::cout << "Exception occurred: " << msg << std::endl;
    }
}
