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

#include <QVector3D>
#include <opencv2/core/core.hpp>

#include "triangle.h"
#include "quad.h"
#include "intersectablelist.h"
#include "material.h"
#include "bvh.h"

#define TOKEN_VERTEX_POS "v"
#define TOKEN_VERTEX_NOR "vn"
#define TOKEN_VERTEX_TEX "vt"
#define TOKEN_FACE "f"

struct ObjMeshVertex{
    QVector3D pos;
    cv::Point2f texcoord;
    QVector3D normal;
};

/* This contains a list of triangles */
struct ObjMesh{
    std::list<Intersectable*> faces;
};

/* Internal structure */
struct _ObjMeshFaceIndex{
    int pos_index[3];
    int tex_index[3];
    int nor_index[3];
};

inline void read_index(std::stringstream& str_stream, int& dest, int total_num)
{
    str_stream >> dest;
    if(dest < 0)
    {
        dest += total_num + 1;
    }
}

void read_indices(_ObjMeshFaceIndex& face_index, int i, std::stringstream& str_stream, int num_positions, int num_texcoords, int num_normals)
{
    char interupt;
    read_index(str_stream, face_index.pos_index[i], num_positions);
    if(str_stream.peek() == '/')
    {
        str_stream >> interupt;
        if(str_stream.peek() == '/')
        {
            face_index.tex_index[i] = 0;
        }
        else
        {
            read_index(str_stream, face_index.tex_index[i], num_texcoords);
        }

        if(str_stream.peek() == '/')
        {
            str_stream >> interupt;
            read_index(str_stream, face_index.nor_index[i], num_normals);
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
}

/* Call this function to load a model, only loads triangulated meshes */
Intersectable *ObjReader::getMesh(std::string filename, Material *material){
    ObjMesh myMesh;

    std::vector<QVector3D>          positions;
    std::vector<cv::Point2f>        texcoords;
    std::vector<QVector3D>          normals;
    std::vector<_ObjMeshFaceIndex>  faces;
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

    std::string line_stream;	// No longer depending on char arrays thanks to: Dale Weiler
    while(std::getline(filestream, line_stream)){
        std::stringstream str_stream(line_stream);
        std::string type_str;
        str_stream >> type_str;
        if(type_str == TOKEN_VERTEX_POS){
            QVector3D pos;
            float val;
            str_stream >> val;
            pos.setX(val);
            str_stream >> val;
            pos.setY(val);
            str_stream >> val;
            pos.setZ(val);
            positions.push_back(pos);
        }else if(type_str == TOKEN_VERTEX_TEX){
            cv::Point2f tex;
            str_stream >> tex.x >> tex.y;
            texcoords.push_back(tex);
        }else if(type_str == TOKEN_VERTEX_NOR){
            QVector3D nor;
            float val;
            str_stream >> val;
            nor.setX(val);
            str_stream >> val;
            nor.setY(val);
            str_stream >> val;
            nor.setZ(val);
            normals.push_back(nor);
        }else if(type_str == TOKEN_FACE){
            _ObjMeshFaceIndex face_index;
            read_indices(face_index, 0, str_stream, positions.size(), texcoords.size(), normals.size());
            read_indices(face_index, 2, str_stream, positions.size(), texcoords.size(), normals.size());
            while(!str_stream.eof())
            {
                face_index.pos_index[1] = face_index.pos_index[2];
                face_index.nor_index[1] = face_index.nor_index[2];
                face_index.tex_index[1] = face_index.tex_index[2];
                read_indices(face_index, 2, str_stream, positions.size(), texcoords.size(), normals.size());
                faces.push_back(face_index);
            }
        }
    }
    // Explicit closing of the file
    filestream.close();

    std::cout << "ObjReader: " << faces.size() << " faces" << std::endl;

    for(size_t i = 0; i < faces.size(); ++i){
        Triangle *face;
        if(faces[i].nor_index[0] == 0 || faces[i].nor_index[1] == 0 || faces[i].nor_index[2] == 0)
        {
            face = new Triangle(positions[faces[i].pos_index[0] - 1], positions[faces[i].pos_index[1] - 1], positions[faces[i].pos_index[2] - 1], material);
        }
        else if(faces[i].tex_index[0] == 0 || faces[i].tex_index[1] == 0 || faces[i].tex_index[2] == 0)
        {
            face = new Triangle(positions[faces[i].pos_index[0] - 1], positions[faces[i].pos_index[1] - 1], positions[faces[i].pos_index[2] - 1], normals[faces[i].nor_index[0] - 1], normals[faces[i].nor_index[1] - 1], normals[faces[i].nor_index[2] - 1], material);
        }
        else
        {
            face = new Triangle(positions[faces[i].pos_index[0] - 1], positions[faces[i].pos_index[1] - 1], positions[faces[i].pos_index[2] - 1], normals[faces[i].nor_index[0] - 1], normals[faces[i].nor_index[1] - 1], normals[faces[i].nor_index[2] - 1], texcoords[faces[i].tex_index[0] - 1], texcoords[faces[i].tex_index[1] - 1], texcoords[faces[i].tex_index[2] - 1], material);
        }
        myMesh.faces.push_back(face);
    }

    return BVHNode::create(new IntersectableList(myMesh.faces), 5);
}
