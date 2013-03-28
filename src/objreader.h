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

class ObjReader
{
  public:
    static ast_intersectable_list getMesh(std::string fileName, ast_material defaultMaterial);

private:
    ObjReader() {}

    void getMaterials(std::string filename);
    void createMaterial(ast_vector3_literal diffuseColor, ast_vector3_literal specularColor, float specularCoefficient, QDir dir, std::string textureFilename, std::string materialName);
    ast_intersectable_list load(std::string fileName, ast_material defaultMaterial);

    std::map<std::string, ast_literal_material> materials;
};

#endif

