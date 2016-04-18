#ifndef CLASSIFIER_H
#define CLASSIFIER_H
#include <list>
#include <vector>
#include <Emotion.h>
#include <FacesDifferencesDatabase.h>

class Classifier
{
    public:
        Classifier(){};
        virtual ~Classifier(){};
        virtual void initialize(FacesDifferencesDatabase* database) = 0;
        virtual Emotion classify(std::vector<float> vec) = 0;
    protected:
    private:
};

#endif // CLASSIFIER_H
