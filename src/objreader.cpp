/**
 * The MIT License
 *
 * Copyright (c) 2010 Wouter Lindenhof (http://limegarden.net)
 * Copyright (c) 2012 Gian Calgeer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "objreader.h"

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <algorithm>

#include <QVector3D>
#include <QDir>
#include <QFileInfo>

#include <opencv2/core/core.hpp>

#define TOKEN_VERTEX_POS "v"
#define TOKEN_VERTEX_NOR "vn"
#define TOKEN_VERTEX_TEX "vt"
#define TOKEN_FACE "f"
#define TOKEN_MATERIAL "usemtl"
#define TOKEN_MATERIAL_LIB "mtllib"

#define TOKEN_NEW_MATERIAL "newmtl"
#define TOKEN_DIFFUSE_COLOR "Kd"
#define TOKEN_DIFFUSE_TEXTURE "map_Kd"
#define TOKEN_SPECULAR_COLOR "Ks"
#define TOKEN_SPECULAR_COEFFICIENT "Ns"

struct ObjMeshVertex{
    QVector3D pos;
    cv::Point2f texcoord;
    QVector3D normal;
};

/* This contains a list of triangles */
typedef std::vector<ast_intersectable> ObjMesh;

/* Internal structure */
struct _ObjMeshFaceIndex{
    int pos_index[3];
    int tex_index[3];
    int nor_index[3];
    ast_material material;
};

inline bool read_index(std::stringstream& str_stream, int& dest, int total_num)
{
    str_stream >> dest;
    if(str_stream.fail()) return false;
    if(dest < 0)
    {
        dest += total_num + 1;
    }
    return true;
}

bool read_indices(_ObjMeshFaceIndex& face_index, int i, std::stringstream& str_stream, int num_positions, int num_texcoords, int num_normals)
{
    char interupt;
    if(!read_index(str_stream, face_index.pos_index[i], num_positions)) return false;
    if(str_stream.peek() == '/')
    {
        str_stream >> interupt;
        if(str_stream.peek() == '/')
        {
            face_index.tex_index[i] = 0;
        }
        else
        {
            if(!read_index(str_stream, face_index.tex_index[i], num_texcoords)) return false;
        }

        if(str_stream.peek() == '/')
        {
            str_stream >> interupt;
            if(!read_index(str_stream, face_index.nor_index[i], num_normals)) return false;
        }
        else
        {
            face_index.nor_index[i] = 0;
        }
    }
    else
    {
        face_index.tex_index[i] = 0;
        face_index.nor_index[i] = 0;
    }
    return true;
}

/* Call this function to load a model */
ast_intersectable_list ObjReader::getMesh(std::string filename, ast_literal_material defaultMaterial){
    std::map<std::string, ast_literal_material> materials;
    ObjMesh myMesh;

    std::vector<ast_vector3_literal>          positions;
    std::vector<ast_vector2_literal>          texcoords;
    std::vector<ast_vector3_literal>          normals;
    std::vector<_ObjMeshFaceIndex>  triangles;
    /**
     * Load file, parse it
     * Lines beginning with:
     * '#'  are comments can be ignored
     * 'v'  are vertices positions (3 floats that can be positive or negative)
     * 'vt' are vertices texcoords (2 floats that can be positive or negative)
     * 'vn' are vertices normals   (3 floats that can be positive or negative)
     * 'f'  are faces, 3 values that contain 3 values which are separated by / and <space>
     */

    std::ifstream filestream;
    filestream.open(filename.c_str());

    QFileInfo objInfo = QFileInfo(QString(filename.c_str()));

    ast_material currentMaterial = defaultMaterial;

    std::string line_stream;	// No longer depending on char arrays thanks to: Dale Weiler
    int num_faces = 0;
    while(std::getline(filestream, line_stream)){
        std::stringstream str_stream(line_stream);
        std::string type_str;
        str_stream >> type_str;
        if(type_str == TOKEN_VERTEX_POS){
            ast_vector3_literal pos;
            str_stream >> pos.x;
            str_stream >> pos.y;
            str_stream >> pos.z;
            positions.push_back(pos);
        }else if(type_str == TOKEN_VERTEX_TEX){
            ast_vector2_literal tex;
            str_stream >> tex.x >> tex.y;
            texcoords.push_back(tex);
        }else if(type_str == TOKEN_VERTEX_NOR){
            ast_vector3_literal nor;
            str_stream >> nor.x;
            str_stream >> nor.y;
            str_stream >> nor.z;
            normals.push_back(nor);
        }else if(type_str == TOKEN_FACE){
            _ObjMeshFaceIndex triangle_index;
            triangle_index.material = currentMaterial;
            read_indices(triangle_index, 0, str_stream, positions.size(), texcoords.size(), normals.size());
            read_indices(triangle_index, 2, str_stream, positions.size(), texcoords.size(), normals.size());
            while(!str_stream.eof())
            {
                triangle_index.pos_index[1] = triangle_index.pos_index[2];
                triangle_index.nor_index[1] = triangle_index.nor_index[2];
                triangle_index.tex_index[1] = triangle_index.tex_index[2];
                if(read_indices(triangle_index, 2, str_stream, positions.size(), texcoords.size(), normals.size()))
                {
                    triangles.push_back(triangle_index);
                }
            }
            num_faces++;
        }else if(type_str == TOKEN_MATERIAL){
            std::string name;
            str_stream >> name;
            currentMaterial = materials[name];
            if(currentMaterial.empty()){
                currentMaterial = defaultMaterial;
            }
        }
        else if(type_str == TOKEN_MATERIAL_LIB){
            std::string mtl_filename;
            str_stream >> mtl_filename;

            getMaterials(objInfo.dir().filePath(QString(mtl_filename.c_str())).toStdString(), materials);
        }
    }
    // Explicit closing of the file
    filestream.close();

    std::cerr << "ObjReader: " << triangles.size() << " triangles, " << num_faces << " faces" << std::endl;

    myMesh.reserve(triangles.size());

    for(size_t i = 0; i < triangles.size(); ++i){
        ast_triangle face;
        face.material = triangles[i].material;

        if(triangles[i].pos_index[0] != 0 && triangles[i].pos_index[1] != 0 && triangles[i].pos_index[2] != 0)
        {
            face.p1 = positions[triangles[i].pos_index[0] - 1];
            face.p2 = positions[triangles[i].pos_index[1] - 1];
            face.p3 = positions[triangles[i].pos_index[2] - 1];
        }
        if(triangles[i].nor_index[0] != 0 && triangles[i].nor_index[1] != 0 && triangles[i].nor_index[2] != 0)
        {
            face.n1 = normals[triangles[i].nor_index[0] - 1];
            face.n2 = normals[triangles[i].nor_index[1] - 1];
            face.n3 = normals[triangles[i].nor_index[2] - 1];
        }
        if(triangles[i].tex_index[0] != 0 && triangles[i].tex_index[1] != 0 && triangles[i].tex_index[2] != 0)
        {
            face.t1 = texcoords[triangles[i].tex_index[0] - 1];
            face.t2 = texcoords[triangles[i].tex_index[1] - 1];
            face.t3 = texcoords[triangles[i].tex_index[2] - 1];
        }
        myMesh.push_back(face);
    }

    ast_intersectable_list m = { myMesh };
    return m;
}

void createMaterial(ast_vector3_literal diffuseColor, ast_vector3_literal specularColor, float specularCoefficient, QDir dir, std::string textureFilename, std::string materialName, std::map<std::string, ast_literal_material> &materials)
{
    if(!textureFilename.empty())
    {
        // This is a fix for broken mtl files that use the Windows path separator convention
        std::replace(textureFilename.begin(), textureFilename.end(), '\\', '/');
        textureFilename = dir.filePath(QString(textureFilename.c_str())).toStdString();
        ast_texture_material texture = {textureFilename};
        materials[materialName] = texture;
    }
    else if(diffuseColor.x != 0 || diffuseColor.y != 0 || diffuseColor.z != 0)
    {
        if((specularColor.x == 0 && specularColor.y == 0 && specularColor.z == 0) || specularCoefficient == 0)
        {
            ast_diffuse_material diffuse = {diffuseColor};
            materials[materialName] = diffuse;
        }
        else
        {
            ast_phong_material phong = {diffuseColor, specularColor, specularCoefficient};
            materials[materialName] = phong;
        }
    }
}

void ObjReader::getMaterials(std::string filename, std::map<std::string, ast_literal_material> &materials)
{
    std::ifstream filestream;
    filestream.open(filename.c_str());

    QFileInfo objInfo = QFileInfo(QString(filename.c_str()));

    std::string current_material_name;
    ast_vector3_literal current_diffuse_color;
    std::string current_texture_filename;
    ast_vector3_literal current_specular_color;
    float current_specular_coefficient = 0;

    std::string line_stream;
    while(std::getline(filestream, line_stream)){
        std::stringstream str_stream(line_stream);
        std::string type_str;
        str_stream >> type_str;

        if(type_str == TOKEN_NEW_MATERIAL){
            createMaterial(current_diffuse_color, current_specular_color, current_specular_coefficient, objInfo.dir(), current_texture_filename, current_material_name, materials);
            str_stream >> current_material_name;
        }
        else if(type_str == TOKEN_DIFFUSE_COLOR){
            str_stream >> current_diffuse_color.x >> current_diffuse_color.y >> current_diffuse_color.z;
        }
        else if(type_str == TOKEN_DIFFUSE_TEXTURE){
            str_stream >> current_texture_filename;
        }
        else if(type_str == TOKEN_SPECULAR_COLOR){
            str_stream >> current_specular_color.x >> current_specular_color.y >> current_specular_color.z;
        }
        else if(type_str == TOKEN_SPECULAR_COEFFICIENT){
            str_stream >> current_specular_coefficient;
        }
    }
    createMaterial(current_diffuse_color, current_specular_color, current_specular_coefficient, objInfo.dir(), current_texture_filename, current_material_name, materials);
}
