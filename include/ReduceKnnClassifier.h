#ifndef REDUCEKNNCLASSIFIER_H
#define REDUCEKNNCLASSIFIER_H

#include <Classifier.h>
#include <DimentionalityReducer.h>


class ReduceKnnClassifier : public Classifier
{
    public:
        ReduceKnnClassifier(DimentionalityReducer* reducer);
        virtual ~ReduceKnnClassifier();
        void initialize(FacesDifferencesDatabase* database);
        Emotion classify(std::vector<float> vec);
    protected:
    private:
        DimentionalityReducer* reducer;
        std::list<std::vector<float> > data;
};

#endif // PCAKNNCLASSIFIER_H
