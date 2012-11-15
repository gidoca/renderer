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
#ifndef SCENEAST_H
#define SCENEAST_H

#include "global.h"

#include <boost/variant/recursive_variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <vector>

#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>

#include <opencv2/core/core.hpp>

struct ast_vector3_literal
{
  ast_vector3_literal() : x(0), y(0), z(0) {}
  float x, y, z;

  QVector3D asQVector() const;
  cv::Vec3f asSpectrum() const;
};

struct ast_vector4_literal
{
  ast_vector4_literal() : x(0), y(0), z(0), w(0) {}
  float x, y, z, w;

  QVector4D asQVector() const;
};

struct ast_diffuse_material
{
  ast_vector3_literal color;
};

struct ast_mirror_material
{
  float coefficient;
};

struct ast_texture_material
{
    std::string filename;
};

struct ast_refractive_material
{
    float coefficient;
};

typedef boost::variant<
    ast_diffuse_material,
    ast_mirror_material,
    ast_texture_material,
    ast_refractive_material,
    std::vector<char>
    > ast_material;

struct ast_matrix_literal
{
  ast_vector4_literal v1, v2, v3, v4;
};

struct ast_matrix_translate
{
  ast_vector3_literal translation_vector;
};

struct ast_matrix_rotate
{
  float angle;
  ast_vector3_literal axis;
};

struct ast_matrix_scale
{
  float factor;
};

typedef boost::variant<
    ast_matrix_literal,
    ast_matrix_translate,
    ast_matrix_rotate,
    ast_matrix_scale
    > ast_basic_matrix;

struct ast_matrix
{
    ast_basic_matrix first;
    std::vector<ast_basic_matrix> mult;

    QMatrix4x4 asQMatrix4x4() const;
};

typedef boost::variant<
    boost::recursive_wrapper<ast_intersectable_list>,
    ast_sphere,
    ast_box,
    ast_quad,
    ast_plane,
    ast_obj,
    boost::recursive_wrapper<ast_instance>
    > ast_intersectable;

struct ast_intersectable_list
{
  std::vector<ast_intersectable> children;
};

struct ast_sphere
{
  ast_vector3_literal center;
  float radius;
  ast_material material;
};

struct ast_box
{
  ast_vector3_literal min, max;
  ast_material material;
};

struct ast_quad
{
  ast_vector3_literal p1, p2, p3, p4;
  ast_material material;
};

struct ast_plane
{
    ast_vector4_literal vector;
    ast_material material;
};

struct ast_obj
{
    std::string filename;
    ast_material material;
};

struct ast_instance
{
  ast_matrix transform;
  ast_intersectable intersectable;
};

struct ast_camera
{
    ast_vector3_literal eye, look_at, up;
    float fov, xres, yres;

    Camera asCamera() const;
};

struct ast_point_light
{
    ast_vector3_literal location, intensity;
};

struct ast_area_light
{
    ast_vector3_literal location, u_direction, v_direction, intensity;
};

struct ast_cone_light
{
    ast_vector3_literal location, direction, intensity;
    float angle;
};

typedef boost::variant<ast_point_light, ast_area_light, ast_cone_light> ast_light;

typedef boost::variant<std::vector<ast_light>, ast_camera, ast_intersectable, ast_material> ast_value;

struct ast_assignment
{
    std::string name;
    ast_value value;
};

BOOST_FUSION_ADAPT_STRUCT(
    ast_intersectable_list,
    (std::vector<ast_intersectable>, children)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_sphere,
    (ast_vector3_literal, center)
    (float, radius)
    (ast_material, material)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_box,
    (ast_vector3_literal, min)
    (ast_vector3_literal, max)
    (ast_material, material)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_quad,
    (ast_vector3_literal, p1)
    (ast_vector3_literal, p2)
    (ast_vector3_literal, p3)
    (ast_vector3_literal, p4)
    (ast_material, material)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_plane,
    (ast_vector4_literal, vector)
    (ast_material, material)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_obj,
    (std::string, filename)
    (ast_material, material)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_instance,
    (ast_matrix, transform)
    (ast_intersectable, intersectable)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_vector3_literal,
    (float, x)
    (float, y)
    (float, z)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_vector4_literal,
    (float, x)
    (float, y)
    (float, z)
    (float, w)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_matrix_literal,
    (ast_vector4_literal, v1)
    (ast_vector4_literal, v2)
    (ast_vector4_literal, v3)
    (ast_vector4_literal, v4)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_matrix,
    (ast_basic_matrix, first)
    (std::vector<ast_basic_matrix>, mult)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_matrix_translate,
    (ast_vector3_literal, translation_vector)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_matrix_rotate,
    (float, angle)
    (ast_vector3_literal, axis)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_matrix_scale,
    (float, factor)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_diffuse_material,
    (ast_vector3_literal, color)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_mirror_material,
    (float, coefficient)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_texture_material,
    (std::string, filename)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_refractive_material,
    (float, coefficient)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_camera,
    (ast_vector3_literal, eye)
    (ast_vector3_literal, look_at)
    (ast_vector3_literal, up)
    (float, fov)
    (float, xres)
    (float, yres)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_point_light,
    (ast_vector3_literal, location)
    (ast_vector3_literal, intensity)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_area_light,
    (ast_vector3_literal, location)
    (ast_vector3_literal, u_direction)
    (ast_vector3_literal, v_direction)
    (ast_vector3_literal, intensity)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_cone_light,
    (ast_vector3_literal, location)
    (ast_vector3_literal, direction)
    (float, angle)
    (ast_vector3_literal, intensity)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_assignment,
    (std::string, name)
    (ast_value, value)
)

Scene buildScene(std::vector<ast_assignment> assignments);

#endif // SCENEAST_H
