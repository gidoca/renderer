#ifndef PATH_H
#define PATH_H

#include <list>

#include "global.h"
#include "hitrecord.h"

#include <opencv2/core/core.hpp>

class Path
{
public:
  std::list<cv::Vec3f> alphaValues;
  std::list<HitRecord> hitRecords;
};

#endif // PATH_H
