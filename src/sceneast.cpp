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
#include "sceneast.h"

#include "sphere.h"
#include "axisalignedbox.h"
#include "quad.h"
#include "plane.h"
#include "intersectableinstance.h"
#include "intersectablelist.h"
#include "diffusematerial.h"
#include "phongmaterial.h"
#include "mirrormaterial.h"
#include "texturematerial.h"
#include "transparentmaterial.h"
#include "camera.h"
#include "scene.h"
#include "pointlight.h"
#include "arealight.h"
#include "conelight.h"
#include "objreader.h"
#include "bvh.h"

#include <boost/foreach.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/get.hpp>

#include <list>
#include <vector>
#include <map>
#include <iostream>

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

    QMatrix4x4 operator()(ast_matrix_scale_vect matrix_scale) const
    {
        QMatrix4x4 matrix;
        matrix.scale(matrix_scale.factor.asQVector());
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
  material_builder(std::map<std::string, Material*>& materials) : materials(materials) {}

  Material* operator()(ast_diffuse_material material) const
  {
    return new DiffuseMaterial(material.color.asSpectrum());
  }

  Material* operator()(ast_phong_material material) const
  {
    return new PhongMaterial(material.diffuse.asSpectrum(), material.specular.asSpectrum(), material.specular_coeff);
  }

  Material* operator()(ast_mirror_material material) const
  {
    return new MirrorMaterial(material.coefficient);
  }

  Material* operator()(ast_texture_material material) const
  {
    TextureMaterial* out = new TextureMaterial();
    if(!out->load(material.filename))
    {
        delete out;
        return DarkMatter::getInstance();
    }
    return out;
  }

  Material* operator()(ast_refractive_material material) const
  {
      return new TransparentMaterial(material.coefficient);
  }

  Material* operator()(std::vector<char> identifier) const
  {
      std::string name = std::string(identifier.begin(), identifier.end());
      Material* material = materials[name];
      if(material == 0)
      {
          std::cout << "No such variable: " << name << std::endl;
          return DarkMatter::getInstance();
      }
      else
      {
          return material;
      }
  }

  Material* operator()(ast_literal_material material) const
  {
      return boost::apply_visitor(*this, material);
  }

  std::map<std::string, Material*>& materials;
};

cv::Point2f ast_vector2_literal::asCVPoint() const
{
    return cv::Point2f(x, y);
}

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
  intersectable_builder(std::map<std::string, Material*>& materials) : materials(materials), material_b(materials) {}

  Sphere* operator()(const ast_sphere& s) const
  {
    return new Sphere(s.center.asQVector(), s.radius, boost::apply_visitor(material_b, s.material));
  }

  AxisAlignedBox* operator()(const ast_box& b) const
  {
    return new AxisAlignedBox(b.min.asQVector(), b.max.asQVector(), boost::apply_visitor(material_b, b.material));
  }

  Quad* operator()(const ast_quad& q) const
  {
    return new Quad(q.p1.asQVector(), q.p2.asQVector(), q.p3.asQVector(), q.p4.asQVector(), boost::apply_visitor(material_b, q.material));
  }

  Plane* operator()(const ast_plane& p) const
  {
      return new Plane(p.vector.asQVector(), boost::apply_visitor(material_b, p.material));
  }

  Triangle* operator()(const ast_triangle& t) const
  {
      return new Triangle(t.p1.asQVector(), t.p2.asQVector(), t.p3.asQVector(), t.n1.asQVector(), t.n2.asQVector(), t.n3.asQVector(), t.t1.asCVPoint(), t.t2.asCVPoint(), t.t3.asCVPoint(), boost::apply_visitor(material_b, t.material));
  }

  IntersectableList* operator()(const ast_intersectable_list& l) const;
  IntersectableInstance* operator()(const ast_instance& i) const;
  Intersectable* operator()(const ast_obj& o) const;

  std::map<std::string, Material*>& materials;

  material_builder material_b;
};

IntersectableList* intersectable_builder::operator()(ast_intersectable_list const& l) const
{
  vector<Intersectable*> intersectables;
  intersectables.reserve(l.children.size());

  BOOST_FOREACH( ast_intersectable n, l.children )
  {
    intersectables.push_back(boost::apply_visitor(*this, n));
  }

  return new IntersectableList(intersectables);
}

IntersectableInstance* intersectable_builder::operator()(const ast_instance& i) const
{
    return new IntersectableInstance(i.transform.asQMatrix4x4(), boost::apply_visitor(*this, i.intersectable));
}

Intersectable* intersectable_builder::operator()(const ast_obj& o) const
{
    ast_intersectable inters = ObjReader::getMesh(o.filename.c_str(), boost::get<ast_literal_material>(o.material));
    Intersectable* mesh = boost::apply_visitor(*this, inters);
    AxisAlignedBox* bb = mesh->boundingBox();
    QVector3D min = bb->getMin(), max = bb->getMax();
    delete bb;
    std::cout << "Mesh bb min: " << min.x() << "," << min.y() << "," << min.z() << "; max: " << max.x() << "," << max.y() << "," << max.z() << std::endl;
    return mesh;
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
    scene_builder() : intersectable_b(materials), material_b(materials) {}

    void addAssignment(ast_assignment assignment)
    {
        current_name = assignment.name;
        deleteAllVars(current_name);
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

    void operator()(ast_literal_material material)
    {
        materials[current_name] = boost::apply_visitor(material_b, material);
    }

    void operator()(std::string identifier)
    {
        if(cameras.count(identifier))
        {
            cameras[current_name] = cameras[identifier];
        }
        else if(intersectables.count(identifier))
        {
            intersectables[current_name] = intersectables[identifier];
        }
        else if(lights.count(identifier))
        {
            lights[current_name] = lights[identifier];
        }
        else if(materials.count(identifier))
        {
            materials[current_name] = materials[identifier];
        }
        else
        {
            std::cout << "Could not assign to " << current_name << ", no such variable: " << identifier << std::endl;
        }
    }

    Scene getScene()
    {
        Scene result(cameras["camera"]);
        result.object = intersectables["intersectable"]->createBVH();
        result.light = lights["lights"];
        return result;
    }

    map<string, Camera> cameras;
    map<string, Intersectable*> intersectables;
    map<string, vector<const Light*> > lights;
    map<string, Material*> materials;

private:
    void deleteAllVars(std::string name)
    {
        cameras.erase(name);
        intersectables.erase(name);
        lights.erase(name);
        materials.erase(name);
    }

    string current_name;

    intersectable_builder intersectable_b;
    material_builder material_b;
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

