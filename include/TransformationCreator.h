#ifndef TRANSFORMATIONCREATOR_H
#define TRANSFORMATIONCREATOR_H
#include <vector>
#include <list>
#include "Transformation.h"


class TransformationCreator
{
    public:
        TransformationCreator();
        virtual ~TransformationCreator();
        Transformation* createOptimalTransformation(std::list<std::vector<float> > vecs);
        std::vector<float> applyTransformation(std::vector<float> vec, Transformation* transformation);
    protected:
    private:
};

#endif // TRANSFORMATIONCREATOR_H
