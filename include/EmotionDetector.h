#define _WINDOWS

#ifndef EMOTIONDETECTOR_H
#define EMOTIONDETECTOR_H
#include <vector>
#include <list>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Emotion.h"
#include "Space.h"
#include "ModelRegistrator.h"
#include "DimentionalityReducer.h"

class EmotionDetector
{
    public:
        EmotionDetector(ModelRegistrator* registrator, DimentionalityReducer* reducer);
        virtual ~EmotionDetector();
        std::vector<float> countDifference(std::list<std::vector<float> > basicFaceExpression, std::list<std::vector<float> > specialFaceExpression);
        Space* createSpaceOfDifferences(std::list<std::vector<float> > differences, std::list<Emotion> emotions);
    protected:
    private:
        ModelRegistrator* registrator;
        DimentionalityReducer* reducer;
};

#endif // EMOTIONDETECTOR_H
