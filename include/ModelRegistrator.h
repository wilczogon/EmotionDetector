#ifndef MODELREGISTRATOR_H
#define MODELREGISTRATOR_H
#include <opencv2/core.hpp>


class ModelRegistrator
{
    public:
        ModelRegistrator(){};
        virtual ~ModelRegistrator(){};
        virtual cv::Mat registerModel(cv::Mat basicModel, cv::Mat registeredModel) = 0;
    protected:
    private:
};

#endif // MODELREGISTRATOR_H
