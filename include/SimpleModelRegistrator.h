#ifndef SIMPLEMODELREGISTRATOR_H
#define SIMPLEMODELREGISTRATOR_H

#include <ModelRegistrator.h>
#include <opencv2/core.hpp>


class SimpleModelRegistrator : public ModelRegistrator
{
    public:
        SimpleModelRegistrator(float maxError);
        virtual ~SimpleModelRegistrator();
        virtual cv::Mat registerModel(cv::Mat basicModel, cv::Mat registeredModel){};
    protected:
    private:
        float maxError;
};

#endif // SIMPLEMODELREGISTRATOR_H
