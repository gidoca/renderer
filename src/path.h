#ifndef PATH_H
#define PATH_H

#include <list>

class Spectrum;
class HitRecord;

class Path
{
public:
  std::list<Spectrum> alphaValues;
  std::list<HitRecord> hitRecords;
};

#endif // PATH_H
