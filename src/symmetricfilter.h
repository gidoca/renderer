#ifndef SYMMETRICFILTER_H
#define SYMMETRICFILTER_H

#include "global.h"

#include <opencv2/core/core.hpp>

class SymmetricFilter
{
public:
    SymmetricFilter() {}

    cv::Mat filter(const cv::Mat & in, const cv::Mat &guide);
};

#endif // SYMMETRICFILTER_H
