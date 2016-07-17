#include "ICPModelRegistrator.h"
#include "opencv2/surface_matching/ppf_helpers.hpp"

ICPModelRegistrator::ICPModelRegistrator()
{
    icp = new cv::ppf_match_3d::ICP();
}

ICPModelRegistrator::~ICPModelRegistrator()
{
    // release icp
}

cv::Mat ICPModelRegistrator::registerModel(cv::Mat basicModel, cv::Mat registeredModel){
    std::vector<cv::ppf_match_3d::Pose3DPtr> poses;
    icp->registerModelToScene(registeredModel, basicModel, poses);

    for(auto pose: poses)
        cv::ppf_match_3d::transformPCPose(registeredModel, pose->pose);

    return registeredModel;
}
