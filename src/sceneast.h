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
#include <boost/fusion/include/equal_to.hpp>

#include <vector>

#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>

#include <opencv2/core/core.hpp>

struct ast_vector2_literal
{
    float x, y;

    cv::Point2f asCVPoint() const;
};

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

  static const std::string function_name;
};

struct ast_phong_material
{
  ast_vector3_literal diffuse;
  ast_vector3_literal specular;
  float specular_coeff;

  static const std::string function_name;
};

struct ast_mirror_material
{
  float coefficient;

  static const std::string function_name;
};

struct ast_texture_material
{
    ast_texture_material()
    {
        coefficient.x = 1;
        coefficient.y = 1;
        coefficient.z = 1;
    }

    std::string filename;
    ast_vector3_literal coefficient;

    static const std::string function_name;
};

struct ast_refractive_material
{
    float coefficient;

    static const std::string function_name;
};

typedef boost::variant<
    ast_diffuse_material,
    ast_phong_material,
    ast_mirror_material,
    ast_texture_material,
    ast_refractive_material
    > ast_literal_material;

typedef boost::variant<
    ast_literal_material,
    std::vector<char>
    > ast_material;

struct ast_matrix_literal
{
  ast_vector4_literal v1, v2, v3, v4;
};

struct ast_matrix_translate
{
  ast_vector3_literal translation_vector;

  static const std::string function_name;
};

struct ast_matrix_rotate
{
  float angle;
  ast_vector3_literal axis;

  static const std::string function_name;
};

struct ast_matrix_scale
{
  float factor;

  static const std::string function_name;
};

struct ast_matrix_scale_vect
{
  ast_vector3_literal factor;

  static const std::string function_name;
};

typedef boost::variant<
    ast_matrix_literal,
    ast_matrix_translate,
    ast_matrix_rotate,
    ast_matrix_scale,
    ast_matrix_scale_vect
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
    ast_triangle,
    boost::recursive_wrapper<ast_instance>,
    boost::recursive_wrapper<ast_bvh_node>
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

  static const std::string function_name;
};

struct ast_box
{
  ast_vector3_literal min, max;
  ast_material material;

  static const std::string function_name;
};

struct ast_quad
{
  ast_vector3_literal p1, p2, p3, p4;
  ast_material material;

  static const std::string function_name;
};

struct ast_plane
{
    ast_vector4_literal vector;
    ast_material material;

    static const std::string function_name;
};

struct ast_obj
{
    std::string filename;
    ast_material material;

    static const std::string function_name;
};

struct ast_triangle
{
    ast_vector3_literal p1, p2, p3, n1, n2, n3;
    ast_vector2_literal t1, t2, t3;
    ast_material material;

    static const std::string function_name;
};

struct ast_instance
{
  ast_matrix transform;
  ast_intersectable intersectable;

  static const std::string function_name;
};

struct ast_bvh_node
{
    ast_intersectable left, right;
    ast_box bb;

    static const std::string function_name;
};

struct ast_camera
{
    ast_vector3_literal eye, look_at, up;
    float fov, xres, yres;

    Camera asCamera() const;

    static const std::string function_name;
};

struct ast_point_light
{
    ast_vector3_literal location, intensity;

    static const std::string function_name;
};

struct ast_area_light
{
    ast_vector3_literal location, u_direction, v_direction, intensity;

    static const std::string function_name;
};

struct ast_cone_light
{
    ast_vector3_literal location, direction, intensity;
    float angle;

    static const std::string function_name;
};

typedef boost::variant<ast_point_light, ast_area_light, ast_cone_light> ast_light;

typedef boost::variant<std::vector<ast_light>, ast_camera, ast_intersectable, ast_literal_material, std::string> ast_value;

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
    ast_triangle,
    (ast_vector3_literal, p1)
    (ast_vector3_literal, p2)
    (ast_vector3_literal, p3)
    (ast_vector3_literal, n1)
    (ast_vector3_literal, n2)
    (ast_vector3_literal, n3)
    (ast_vector2_literal, t1)
    (ast_vector2_literal, t2)
    (ast_vector2_literal, t3)
    (ast_material, material)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_instance,
    (ast_matrix, transform)
    (ast_intersectable, intersectable)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_bvh_node,
    (ast_intersectable, left)
    (ast_intersectable, right)
    (ast_box, bb)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_vector2_literal,
    (float, x)
    (float, y)
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
    ast_matrix_scale_vect,
    (ast_vector3_literal, factor)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_diffuse_material,
    (ast_vector3_literal, color)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_phong_material,
    (ast_vector3_literal, diffuse)
    (ast_vector3_literal, specular)
    (float, specular_coeff)
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

void resolveVars(std::vector<ast_assignment> & assignments);
std::vector<ast_assignment> createBVH(std::vector<ast_assignment> assignments);
Scene buildScene(std::vector<ast_assignment> assignments);

AxisAlignedBox* getBoundingBoxFromAst(ast_intersectable i);

#endif // SCENEAST_H
