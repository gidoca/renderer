#include "sceneast.h"

#include "sphere.h"
#include "axisalignedbox.h"
#include "quad.h"
#include "intersectableinstance.h"
#include "intersectablelist.h"
#include "diffusematerial.h"
#include "mirrormaterial.h"
#include "spectrum.h"

#include <boost/foreach.hpp>
#include <boost/variant/apply_visitor.hpp>

#include <list>

#include <QMatrix4x4>

using namespace std;

struct matrix_evaluator : boost::static_visitor<QMatrix4x4>
{
    QMatrix4x4 operator()(ast_matrix_literal literal) const
    {
        QMatrix4x4 matrix;
        matrix.setRow(0, literal.v1.asQVector());
        matrix.setRow(1, literal.v2.asQVector());
        matrix.setRow(2, literal.v3.asQVector());
        matrix.setRow(3, literal.v4.asQVector());
        return matrix;
    }

    QMatrix4x4 operator()(ast_matrix_translate matrix_translate) const
    {
      QMatrix4x4 matrix;
      matrix.translate(matrix_translate.translation_vector.asQVector());
      return matrix;
    }

    QMatrix4x4 operator()(ast_matrix_rotate matrix_rot) const
    {
      QMatrix4x4 matrix;
      matrix.rotate(matrix_rot.angle, matrix_rot.axis.asQVector());
      return matrix;
    }
};

QMatrix4x4 ast_matrix::asQMatrix4x4() const
{
    QMatrix4x4 first = boost::apply_visitor(matrix_evaluator(), this->first);
    BOOST_FOREACH(ast_basic_matrix mat, this->mult)
    {
        first *= boost::apply_visitor(matrix_evaluator(), mat);
    }

    return first;
}

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

QVector3D ast_vector3_literal::asQVector() const
{
  return QVector3D(x, y, z);
}

cv::Vec3f ast_vector3_literal::asSpectrum() const
{
  return cv::Vec3f(x, y, z);
}

QVector4D ast_vector4_literal::asQVector() const
{
  return QVector4D(x, y, z, w);
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
  Intersectable* operator()(const ast_instance& i) const;
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

Intersectable* scene_builder::operator()(const ast_instance& i) const
{
    return new IntersectableInstance(i.transform.asQMatrix4x4(), boost::apply_visitor(scene_builder(), i.intersectable));
}

Intersectable* buildScene(ast_intersectable n)
{
  return boost::apply_visitor(scene_builder(), n);
}

