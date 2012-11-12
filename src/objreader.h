#ifndef OBJREADER_H
#define OBJREADER_H

#include "global.h"

#include <string>
#include <map>

class ObjReader
{
  public:
    static Intersectable* getMesh(std::string fileName, Material * defaultMaterial, std::map<std::string, Material*> materials);
    static void getMaterials(std::string filename, std::map<std::string, Material*> &materials);
};

#endif

