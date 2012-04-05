#include "sceneast.h"

#include "sphere.h"
#include "diffusematerial.h"
#include "intersectablelist.h"

#include <boost/foreach.hpp>
#include <boost/variant/apply_visitor.hpp>

#include <list>

using namespace std;

QVector3D vctor::asQVector() const
{
  return QVector3D(x, y, z);
}

Sphere sphere::asSphere() const
{
  return Sphere(center.asQVector(), radius, new DiffuseMaterial(Spectrum(1, 1, 1)));
}


struct scene_builder : boost::static_visitor<Intersectable*>
{
  Intersectable* operator()(const sphere& s) const
  {
    return new Sphere(s.center.asQVector(), s.radius, new DiffuseMaterial(Spectrum(1, 1, 1)));
  }

  Intersectable* operator()(const lst& l) const;
};

Intersectable* scene_builder::operator()(lst const& l) const
{
  list<Intersectable*> intersectables;

  BOOST_FOREACH( node n, l.children )
  {
    intersectables.push_back(boost::apply_visitor(scene_builder(), n));
  }

  return new IntersectableList(intersectables);
}

Intersectable* buildScene(node n)
{
  return boost::apply_visitor(scene_builder(), n);
}

