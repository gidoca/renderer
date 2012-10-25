#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <vector>
#include <opencv2/core/core.hpp>

void var(cv::Mat &mean, cv::Mat &var, std::vector<cv::Mat> in);
cv::Mat channelMean(const cv::Mat &in);
cv::Mat channelNorm(const cv::Mat &in);
cv::Mat extend(const cv::Mat &in, int nchan = 3);

template <typename T> inline constexpr
int signum(T x, std::false_type is_signed) {
    return T(0) < x;
}

template <typename T> inline constexpr
int signum(T x, std::true_type is_signed) {
    return (T(0) < x) - (x < T(0));
}

template <typename T> inline constexpr
int signum(T x) {
    return signum(x, std::is_signed<T>());
}

#endif
