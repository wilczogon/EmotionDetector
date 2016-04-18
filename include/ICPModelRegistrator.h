#ifndef ICPMODELREGISTRATOR_H
#define ICPMODELREGISTRATOR_H

#include <ModelRegistrator.h>
#include <opencv2/core.hpp>


class ICPModelRegistrator : public ModelRegistrator
{
    public:
        ICPModelRegistrator();
        ~ICPModelRegistrator();
        cv::Mat registerModel(cv::Mat basicModel, cv::Mat registeredModel);
    protected:
    private:
};

#endif // ICPMODELREGISTRATOR_H
