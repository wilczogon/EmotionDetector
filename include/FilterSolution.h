#ifndef FILTERSOLUTION_H
#define FILTERSOLUTION_H
#include <opencv2/core.hpp>

class FilterSolution
{
    public:
        FilterSolution(float lowerThreshold, float upperThreshold);
        void filter(cv::Mat &data);
        virtual ~FilterSolution();
        static float countMaxVectorLen(cv::Mat data);
    protected:
    private:
        float lowerThreshold;
        float upperThreshold;
};

#endif // FILTERSOLUTION_H
