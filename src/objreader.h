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
#ifndef OBJREADER_H
#define OBJREADER_H

#include "global.h"

#include "sceneast.h"

#include <string>
#include <map>

#include <QDir>

struct ObjMaterial
{
    ast_vector3_literal diffuse_color;
    std::string texture_filename;
    ast_vector3_literal specular_color;
    float specular_coefficient;
    float optical_density;
    short illum;
};


class ObjReader
{
  public:
    ast_intersectable_list load(std::string fileName, ast_material defaultMaterial);

    inline std::map<std::string, ast_literal_material> getMaterials() const
    {
        return materials;
    }

private:
    void getMaterials(std::string filename);
    void createMaterial(ObjMaterial material, QDir dir, std::string materialName);

    std::map<std::string, ast_literal_material> materials;
};

#endif

