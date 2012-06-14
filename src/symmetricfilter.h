#ifndef SYMMETRICFILTER_H
#define SYMMETRICFILTER_H

#include "global.h"

#include <opencv2/core/core.hpp>

class SymmetricFilter
{
public:
    SymmetricFilter() {}

    cv::Mat filter(const cv::Mat & image, const cv::Mat &guide, const cv::Mat &pixvar);
};

#endif // SYMMETRICFILTER_H
