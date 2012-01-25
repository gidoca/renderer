#ifndef PATH_H
#define PATH_H

#include <list>

#include "global.h"
#include "hitrecord.h"
#include "spectrum.h"

class Path
{
public:
  std::list<Spectrum> alphaValues;
  std::list<HitRecord> hitRecords;
};

#endif // PATH_H
