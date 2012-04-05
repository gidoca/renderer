#ifndef SCENEAST_H
#define SCENEAST_H

#include "global.h"

#include <boost/variant/recursive_variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <vector>

#include <QVector3D>

typedef boost::variant<
    boost::recursive_wrapper<lst>,
    sphere
    > node;

struct lst
{
  std::vector<node> children;
};

struct vctor
{
  float x, y, z;

  QVector3D asQVector() const;
};

struct sphere
{
  vctor center;
  float radius;

  Sphere asSphere() const;
};

BOOST_FUSION_ADAPT_STRUCT(
    lst,
    (std::vector<node>, children)
)

BOOST_FUSION_ADAPT_STRUCT(
    sphere,
    (vctor, center)
    (float, radius)
)

BOOST_FUSION_ADAPT_STRUCT(
    vctor,
    (float, x)
    (float, y)
    (float, z)
)

Intersectable* buildScene(node n);

#endif // SCENEAST_H
