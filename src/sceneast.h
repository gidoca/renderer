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
  float x, y, z;

  QVector3D asQVector() const;
  cv::Vec3f asSpectrum() const;
};

struct ast_vector4_literal
{
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

typedef boost::variant<
    ast_diffuse_material,
    ast_mirror_material
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

typedef boost::variant<
    ast_matrix_literal,
    ast_matrix_translate,
    ast_matrix_rotate
    > ast_basic_matrix;

struct ast_matrix
{
    ast_basic_matrix first;
    std::vector<ast_basic_matrix> mult;

    QMatrix4x4 asQMatrix4x4() const;
};

typedef boost::variant<
    boost::recursive_wrapper<ast_list>,
    ast_sphere,
    ast_box,
    ast_quad,
    boost::recursive_wrapper<ast_instance>
    > ast_intersectable;

struct ast_list
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

struct ast_instance
{
  ast_matrix transform;
  ast_intersectable intersectable;
};

BOOST_FUSION_ADAPT_STRUCT(
    ast_list,
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
    ast_diffuse_material,
    (ast_vector3_literal, color)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_mirror_material,
    (float, coefficient)
)

Intersectable* buildScene(ast_intersectable n);

#endif // SCENEAST_H
