#ifndef DIMENTIONALITYREDUCER_H
#define DIMENTIONALITYREDUCER_H
#include <list>
#include <vector>


class DimentionalityReducer
{
    public:
        DimentionalityReducer(int dimensionsNo);
        virtual ~DimentionalityReducer() = 0;
        virtual void initialize(std::list<std::vector<float> > data) = 0;
        virtual std::list<std::vector<float> > reduceDimentionality(std::list<std::vector<float> > data) = 0;
        virtual std::vector<float> reduceDimentionality(std::vector<float> vec) = 0;
        int getNumberOfDimensions(){return dimensionsNo;};
    protected:
        int dimensionsNo;
    private:
};

#endif // DIMENTIONALITYREDUCER_H
