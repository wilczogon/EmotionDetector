#ifndef PCADIMENTIONALITYREDUCER_H
#define PCADIMENTIONALITYREDUCER_H

#include <DimentionalityReducer.h>
#include <opencv/cv.h>

using namespace cv;

class PCADimentionalityReducer : public DimentionalityReducer
{
    public:
        PCADimentionalityReducer(int dimensionsNo);
        virtual ~PCADimentionalityReducer();
        virtual void initialize(std::list<std::vector<float> > data);
        virtual std::list<std::vector<float> > reduceDimentionality(std::list<std::vector<float> > data);
        virtual std::vector<float> reduceDimentionality(std::vector<float> vec);
    protected:
    private:
        PCA pca;
};

#endif // PCADIMENTIONALITYREDUCER_H
