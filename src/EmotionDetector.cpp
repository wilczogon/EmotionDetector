#include "EmotionDetector.h"
#include "Visualizer.h"
#include "FileStream.h"
#include "FacialLandmarkDetector.h"
#include "DlibFacialLandmarkDetector.h"
#include "ModelRegistrator.h"
#include "SimpleModelRegistrator.h"
#include "DimentionalityReducer.h"
#include "PCADimentionalityReducer.h"
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <dlib/image_processing.h>
#include <dlib/image_io.h>

using namespace dlib;
using namespace std;

EmotionDetector::EmotionDetector(ModelRegistrator* registrator, DimentionalityReducer* reducer)
{
    this->registrator = registrator;
    this->reducer = reducer;
}

EmotionDetector::~EmotionDetector()
{
    //dtor
}

std::vector<float> EmotionDetector::countDifference(std::list<std::vector<float> > basicFaceExpression, std::list<std::vector<float> > specialFaceExpression){

    if(basicFaceExpression.size() != specialFaceExpression.size()){
        throw "Not equal number of points";
    }

    std::vector<float> result;
    std::list<std::vector<float> >::iterator basicIt;
    std::list<std::vector<float> >::iterator expIt;

    for(basicIt = basicFaceExpression.begin(), expIt = specialFaceExpression.begin();
        basicIt != basicFaceExpression.end(), expIt != specialFaceExpression.end();
        ++basicIt, ++expIt){

            if((*basicIt).size()!=(*expIt).size())
                throw "Points with different numbers of coordinates";

            if((*basicIt).size() < 1 || (*expIt).size() < 1)
                throw "Zero size points.";
    }

    basicFaceExpression = registrator->registerModel(specialFaceExpression, basicFaceExpression);

    for(basicIt = basicFaceExpression.begin(), expIt = specialFaceExpression.begin();
        basicIt != basicFaceExpression.end() || expIt != specialFaceExpression.end();
        ++basicIt, ++expIt){

            for(unsigned int i = 0; i<(*basicIt).size(); ++i){
                result.push_back((*expIt)[i] - (*basicIt)[i]);
            }
    }

    //TODO colors?

    return result;

}

Space* EmotionDetector::createSpaceOfDifferences(std::list<std::vector<float> > differences, std::list<Emotion> emotions){
    if(differences.size() != emotions.size())
        throw "Not equal number of differences and emotions";

    reducer->initialize(differences, 5);

    std::list<void*> data;
    std::list<Emotion>::iterator iter;
    for(iter = emotions.begin(); iter != emotions.end(); ++iter)
        data.push_back((void*)(*iter));

    return new Space(reducer->reduceDimentionality(differences), data);
}
/*
int main(){
    DimentionalityReducer* reducer = new PCADimentionalityReducer();
    ModelRegistrator* registrator = new SimpleModelRegistrator(0.5);
    EmotionDetector* detector = new EmotionDetector(registrator, reducer);

    std::string fileName = "C:\\Users\\Mary\\Documents\\Mary\\9_semestr\\magisterka\\project\\test_images\\gibbs2.jpg";

    FacialLandmarkDetector* fdet = new DlibFacialLandmarkDetector();
    std::list<std::list<std::vector<float> > > res = fdet->getFacesPoints(fileName);
    std::list<std::list<std::vector<float> > > res2 = fdet->getFacesPoints("C:\\Users\\Mary\\Documents\\Mary\\9_semestr\\magisterka\\project\\test_images\\gibbs.jpg");

    std::vector<float> diff = detector->countDifference(*res.begin(), *res2.begin());

    std::list<std::vector<float> > diffs;
    std::list<Emotion> emotions;
    diffs.push_back(diff);
    diffs.push_back(diff);
    emotions.push_back(neutral);
    emotions.push_back(sad);
    Space* space = detector->createSpaceOfDifferences(diffs, emotions);

    Visualizer* visualizer = new Visualizer();

    std::list<sf::Color> colors;
    for(Emotion &emotion: emotions){
        colors.push_back(sf::Color(
                                    emotion%3 == 0? 255:0,
                                    emotion%3 == 1? 255:0,
                                    emotion%3 == 2? 255:0
                                   ));
    }

    visualizer->visualizeClusters("Clusters", diffs, colors, 800, 600);


    std::list<std::vector<float> > res3 = registrator->registerModel((*res2.begin()), (*res.begin()));
    FileStream stream;
    stream.open("C:\\Users\\Mary\\Documents\\Mary\\9_semestr\\magisterka\\project\\test_images\\gibbs.jpg");

    sf::Texture texture;
    texture.loadFromStream(stream);
    for(std::list<std::vector<float> > &lv : res2){
        for(std::vector<float> &v : lv){
            visualizer->addPoint(sf::Vector2f((int)v[0], (int)v[1]), sf::Color(255, 0, 0));
        }
    }
    for(std::vector<float> &v : res3){
        visualizer->addPoint(sf::Vector2f((int)v[0], (int)v[1]), sf::Color(255, 255, 0));
    }

    visualizer->visualize("Test", 640, 480, &texture);

    return 0;
}
*/
