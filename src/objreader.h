#ifndef OBJREADER_H
#define OBJREADER_H

#include "global.h"

#include <string>

class ObjReader
{
  public:
    static Intersectable* getMesh(std::string fileName, Material * material);
};

#endif

