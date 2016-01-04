#ifndef MODELREGISTRATOR_H
#define MODELREGISTRATOR_H
#include <list>
#include <vector>


class ModelRegistrator
{
    public:
        ModelRegistrator(){};
        virtual ~ModelRegistrator(){};
        virtual std::list<std::vector<float> > registerModel(std::list<std::vector<float> > basicModel, std::list<std::vector<float> > registeredModel) = 0;
    protected:
    private:
};

#endif // MODELREGISTRATOR_H
