#ifndef ICPMODELREGISTRATOR_H
#define ICPMODELREGISTRATOR_H

#include <ModelRegistrator.h>


class ICPModelRegistrator : public ModelRegistrator
{
    public:
        ICPModelRegistrator();
        ~ICPModelRegistrator();
        std::list<std::vector<float> > registerModel(std::list<std::vector<float> > basicModel, std::list<std::vector<float> > registeredModel);
    protected:
    private:
};

#endif // ICPMODELREGISTRATOR_H
