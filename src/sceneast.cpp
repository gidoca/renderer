#include "sceneast.h"

#include "sphere.h"
#include "axisalignedbox.h"
#include "quad.h"
#include "plane.h"
#include "intersectableinstance.h"
#include "intersectablelist.h"
#include "diffusematerial.h"
#include "mirrormaterial.h"
#include "texturematerial.h"
#include "camera.h"
#include "scene.h"
#include "pointlight.h"
#include "arealight.h"
#include "conelight.h"
#include "objreader.h"

#include <boost/foreach.hpp>
#include <boost/variant/apply_visitor.hpp>

#include <list>
#include <map>

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

    QMatrix4x4 operator()(ast_matrix_scale matrix_scale) const
    {
        QMatrix4x4 matrix;
        matrix.scale(matrix_scale.factor);
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

  Material* operator()(ast_texture_material material) const
  {
    TextureMaterial* out = new TextureMaterial();
    out->load(material.filename);
    return out;
  }
};

QVector3D ast_vector3_literal::asQVector() const
{
  return QVector3D(x, y, z);
}

cv::Vec3f ast_vector3_literal::asSpectrum() const
{
  //OpenCV uses BGR byte order
  return cv::Vec3f(z, y, x);
}

QVector4D ast_vector4_literal::asQVector() const
{
  return QVector4D(x, y, z, w);
}

Camera ast_camera::asCamera() const
{
    return Camera(eye.asQVector(), look_at.asQVector(), up.asQVector(), fov, QSize(xres, yres));
}

struct intersectable_builder : boost::static_visitor<Intersectable*>
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

  Intersectable* operator()(const ast_plane& p) const
  {
      return new Plane(p.vector.asQVector(), boost::apply_visitor(material_builder(), p.material));
  }

  Intersectable* operator()(const ast_obj& o) const
  {
      return ObjReader::getMesh(o.filename.c_str(), boost::apply_visitor(material_builder(), o.material));
  }

  Intersectable* operator()(const ast_intersectable_list& l) const;
  Intersectable* operator()(const ast_instance& i) const;
};

Intersectable* intersectable_builder::operator()(ast_intersectable_list const& l) const
{
  list<Intersectable*> intersectables;

  BOOST_FOREACH( ast_intersectable n, l.children )
  {
    intersectables.push_back(boost::apply_visitor(intersectable_builder(), n));
  }

  return new IntersectableList(intersectables);
}

Intersectable* intersectable_builder::operator()(const ast_instance& i) const
{
    return new IntersectableInstance(i.transform.asQMatrix4x4(), boost::apply_visitor(intersectable_builder(), i.intersectable));
}

struct light_builder : boost::static_visitor<const Light*>
{
    const Light* operator()(ast_point_light point_light) const
    {
        return new PointLight(point_light.location.asQVector(), point_light.intensity.asSpectrum());
    }

    const Light* operator()(ast_area_light area_light) const
    {
        return new AreaLight(area_light.location.asQVector(), area_light.u_direction.asQVector(), area_light.v_direction.asQVector(), area_light.intensity.asSpectrum());
    }

    const Light* operator()(ast_cone_light cone_light) const
    {
        return new ConeLight(cone_light.location.asQVector(), cone_light.direction.asQVector(), cone_light.angle, cone_light.intensity.asSpectrum());
    }
};

struct scene_builder : boost::static_visitor<void>
{
    void addAssignment(ast_assignment assignment)
    {
        current_name = assignment.name;
        boost::apply_visitor(*this, assignment.value);
    }

    void operator()(ast_camera camera)
    {
        cameras[current_name] = camera.asCamera();
    }

    void operator()(ast_intersectable intersectable)
    {
        intersectables[current_name] = boost::apply_visitor(intersectable_b, intersectable);
    }

    void operator()(std::vector<ast_light> lights)
    {
        vector<const Light*> out;
        BOOST_FOREACH(ast_light light, lights)
        {
            out.push_back(boost::apply_visitor(light_b, light));
        }
        this->lights[current_name] = out;
    }

    Scene getScene()
    {
        Scene result(cameras["camera"]);
        result.object = intersectables["intersectable"];
        result.light = lights["lights"];
        return result;
    }

    map<string, Camera> cameras;
    map<string, Intersectable*> intersectables;
    map<string, vector<const Light*> > lights;

private:
    string current_name;

    intersectable_builder intersectable_b;
    light_builder light_b;
};

Scene buildScene(vector<ast_assignment> assignments)
{
  scene_builder builder;
  BOOST_FOREACH(ast_assignment assignment, assignments)
  {
    builder.addAssignment(assignment);
  }
  return builder.getScene();
}

