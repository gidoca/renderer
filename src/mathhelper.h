/**
 * Copyright (C) 2012 Gian Calgeer
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in the
 * Software without restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat reduceval(cv::Mat in, int rtype);
cv::Mat minval(cv::Mat in);
cv::Mat maxval(cv::Mat in);

void var(cv::Mat &mean, cv::Mat &var, const std::vector<cv::Mat>& in);
cv::Mat channelMean(const cv::Mat &in);
cv::Mat channelNorm(const cv::Mat &in);
cv::Mat extend(const cv::Mat &in, int nchan = 3);

void mean(cv::Mat &out, std::vector<cv::Mat> in);

template <typename T> inline constexpr
int signum(T x, std::false_type ) {
    return T(0) < x;
}

template <typename T> inline constexpr
int signum(T x, std::true_type ) {
    return (T(0) < x) - (x < T(0));
}

template <typename T> inline constexpr
int signum(T x) {
    return signum(x, std::is_signed<T>());
}

float lum(cv::Vec3f in);

#endif
