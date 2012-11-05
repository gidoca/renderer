#ifndef SYMMETRICFILTER_H
#define SYMMETRICFILTER_H

#include "global.h"

#include <opencv2/core/core.hpp>

class SymmetricFilter
{
public:
    SymmetricFilter(int patchSize = 7, int windowRadius = 10) : patchSize(patchSize), windowRadius(windowRadius) {}

    cv::Mat filter(const cv::Mat & image, const cv::Mat &guide, const cv::Mat &pixvar);

    const int patchSize;
    const int windowRadius;

};

#endif // SYMMETRICFILTER_H
