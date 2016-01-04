#ifndef SIMPLEMODELREGISTRATOR_H
#define SIMPLEMODELREGISTRATOR_H

#include <ModelRegistrator.h>


class SimpleModelRegistrator : public ModelRegistrator
{
    public:
        SimpleModelRegistrator(float maxError);
        virtual ~SimpleModelRegistrator();
        virtual std::list<std::vector<float> > registerModel(std::list<std::vector<float> > basicModel, std::list<std::vector<float> > registeredModel);
    protected:
    private:
        float maxError;
};

#endif // SIMPLEMODELREGISTRATOR_H
