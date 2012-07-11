#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <vector>
#include <opencv2/core/core.hpp>

void var(cv::Mat &mean, cv::Mat &var, std::vector<cv::Mat> in);
cv::Mat channelMean(const cv::Mat &in);

#endif
