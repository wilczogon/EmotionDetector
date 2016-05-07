#ifndef ICPMODELREGISTRATOR_H
#define ICPMODELREGISTRATOR_H

#include <ModelRegistrator.h>
#include <opencv2/core.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/surface_matching.hpp>


class ICPModelRegistrator : public ModelRegistrator
{
    public:
        ICPModelRegistrator();
        ~ICPModelRegistrator();
        cv::Mat registerModel(cv::Mat basicModel, cv::Mat registeredModel);
    protected:
    private:
        cv::ppf_match_3d::ICP* icp;
};

#endif // ICPMODELREGISTRATOR_H
