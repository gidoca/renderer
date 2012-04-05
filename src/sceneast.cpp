#include "sceneast.h"

#include "sphere.h"
#include "axisalignedbox.h"
#include "quad.h"
#include "diffusematerial.h"
#include "mirrormaterial.h"
#include "intersectablelist.h"
#include "spectrum.h"

#include <boost/foreach.hpp>
#include <boost/variant/apply_visitor.hpp>

#include <list>

using namespace std;

struct material_builder : boost::static_visitor<Material*>
{
  Material* operator()(ast_diffuse_material material) const
  {
    return new DiffuseMaterial(material.color.asSpectrum());
  }

  Material* operator()(ast_mirror_material material) const
  {
    return new MirrorMaterial(material.coefficient);
  }
};

QVector3D ast_vector::asQVector() const
{
  return QVector3D(x, y, z);
}

Spectrum ast_vector::asSpectrum() const
{
  return Spectrum(x, y, z);
}

struct scene_builder : boost::static_visitor<Intersectable*>
{
  Intersectable* operator()(const ast_sphere& s) const
  {
    return new Sphere(s.center.asQVector(), s.radius, boost::apply_visitor(material_builder(), s.material));
  }

  Intersectable* operator()(const ast_box& b) const
  {
    return new AxisAlignedBox(b.min.asQVector(), b.max.asQVector(), boost::apply_visitor(material_builder(), b.material));
  }

  Intersectable* operator()(const ast_quad& q) const
  {
    return new Quad(q.p1.asQVector(), q.p2.asQVector(), q.p3.asQVector(), q.p4.asQVector(), boost::apply_visitor(material_builder(), q.material));
  }

  Intersectable* operator()(const ast_list& l) const;
};

Intersectable* scene_builder::operator()(ast_list const& l) const
{
  list<Intersectable*> intersectables;

  BOOST_FOREACH( ast_intersectable n, l.children )
  {
    intersectables.push_back(boost::apply_visitor(scene_builder(), n));
  }

  return new IntersectableList(intersectables);
}

Intersectable* buildScene(ast_intersectable n)
{
  return boost::apply_visitor(scene_builder(), n);
}

