#ifndef SCENEAST_H
#define SCENEAST_H

#include "global.h"

#include <boost/variant/recursive_variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <vector>

#include <QVector3D>

typedef boost::variant<
    boost::recursive_wrapper<ast_list>,
    ast_sphere,
    ast_box,
    ast_quad
    > ast_intersectable;

struct ast_list
{
  std::vector<ast_intersectable> children;
};

struct ast_vector
{
  float x, y, z;

  QVector3D asQVector() const;
  Spectrum asSpectrum() const;
};

struct ast_diffuse_material
{
  ast_vector color;
};

struct ast_mirror_material
{
  float coefficient;
};

typedef boost::variant<
    ast_diffuse_material,
    ast_mirror_material
    > ast_material;

struct ast_sphere
{
  ast_vector center;
  float radius;
  ast_material material;
};

struct ast_box
{
  ast_vector min, max;
  ast_material material;
};

struct ast_quad
{
  ast_vector p1, p2, p3, p4;
  ast_material material;
};

BOOST_FUSION_ADAPT_STRUCT(
    ast_list,
    (std::vector<ast_intersectable>, children)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_sphere,
    (ast_vector, center)
    (float, radius)
    (ast_material, material)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_box,
    (ast_vector, min)
    (ast_vector, max)
    (ast_material, material)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_quad,
    (ast_vector, p1)
    (ast_vector, p2)
    (ast_vector, p3)
    (ast_vector, p4)
    (ast_material, material)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_vector,
    (float, x)
    (float, y)
    (float, z)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_diffuse_material,
    (ast_vector, color)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast_mirror_material,
    (float, coefficient)
)

Intersectable* buildScene(ast_intersectable n);

#endif // SCENEAST_H
