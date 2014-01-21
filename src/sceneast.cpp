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
#include "environmentmap.h"
#include "blacklight.h"
#include "objreader.h"
#include "bvh.h"
#include "csgobject.h"
#include "csgoperation.h"

#include <boost/foreach.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/get.hpp>
#include <boost/fusion/include/equal_to.hpp>
#include <boost/functional/hash.hpp>
#include <boost/fusion/algorithm/iteration/fold.hpp>

#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <iostream>
#include <algorithm>

#include <QMatrix4x4>

using namespace std;

const std::string ast_diffuse_material::function_name = "diffuse";
const std::string ast_phong_material::function_name = "phong";
const std::string ast_mirror_material::function_name = "mirror";
const std::string ast_texture_material::function_name = "texture";
const std::string ast_scaled_texture_material::function_name = "texture";
const std::string ast_refractive_material::function_name = "refractive";
const std::string ast_matrix_translate::function_name = "translate";
const std::string ast_matrix_rotate::function_name = "rotate";
const std::string ast_matrix_scale::function_name = "scale";
const std::string ast_matrix_scale_vect::function_name = "scale";
const std::string ast_sphere::function_name = "sphere";
const std::string ast_box::function_name = "box";
const std::string ast_quad::function_name = "quad";
const std::string ast_plane::function_name = "plane";
const std::string ast_obj::function_name = "obj";
const std::string ast_triangle::function_name = "t";
const std::string ast_instance::function_name = "instance";
const std::string ast_bvh_node::function_name = "b";
const std::string ast_csg_isect::function_name = "isect";
const std::string ast_csg_union::function_name = "union";
const std::string ast_csg_difference::function_name = "diff";
const std::string ast_csg_xor::function_name = "xor";
const std::string ast_camera::function_name = "camera";
const std::string ast_point_light::function_name = "pointlight";
const std::string ast_area_light::function_name = "srealight";
const std::string ast_cone_light::function_name = "conelight";
const std::string ast_environment_map::function_name = "environmentmap";


template<typename V>
struct IntersectableAssignmentVisitor : boost::static_visitor<ast_value>
{
    IntersectableAssignmentVisitor() {}

    IntersectableAssignmentVisitor(V v) : v(v) {}

    void apply(ast_assignment &a)
    {
        a.value = boost::apply_visitor(*this, a.value);
    }

    void applyAll(std::vector<ast_assignment> &as)
    {
        for(ast_assignment& a: as)
        {
            apply(a);
        }
    }

    ast_value operator()(ast_intersectable i)
    {
        return ast_intersectable(boost::apply_visitor(v, i));
    }

    template<typename T>
    ast_value operator()(T t)
    {
        return ast_value(t);
    }

    V getVisitor()
    {
        return v;
    }

private:
    V v;
};

struct ObjLoader : boost::static_visitor<ast_intersectable>
{
    ast_intersectable_list operator()(ast_intersectable_list l)
    {
        for(unsigned int i = 0; i < l.children.size(); i++)
        {
            l.children[i] = boost::apply_visitor(*this, l.children[i]);
        }
        return l;
    }

    ast_instance operator()(ast_instance i)
    {
        i.intersectable = boost::apply_visitor(*this, i.intersectable);
        return i;
    }

    ast_bvh_node operator()(ast_bvh_node b)
    {
        b.left = boost::apply_visitor(*this, b.left);
        b.right = boost::apply_visitor(*this, b.right);
        return b;
    }

    ast_intersectable_list operator()(ast_obj o)
    {
        ObjReader reader;
        ast_intersectable_list result = reader.load(o.filename, o.material);
        std::map<std::string, ast_literal_material> newMaterials = reader.getMaterials();
        materials.insert(newMaterials.begin(), newMaterials.end());
        return result;
    }

    template<typename T>
    ast_intersectable operator()(T t)
    {
        return t;
    }

    inline std::map<std::string, ast_literal_material> getMaterials()
    {
        return materials;
    }

private:
    std::map<std::string, ast_literal_material> materials;
};

struct BVHCreator : boost::static_visitor<ast_intersectable>
{
    ast_intersectable operator()(ast_instance i) const
    {
        i.intersectable = boost::apply_visitor(*this, i.intersectable);
        return i;
    }

    ast_intersectable operator()(ast_bvh_node b) const
    {
        return b;
    }

    ast_intersectable operator()(ast_intersectable_list l) const
    {
        return BVHNode::create(l);
    }

    template<typename T>
    ast_intersectable operator()(T t) const
    {
        return t;
    }
};

struct Flattener : boost::static_visitor<ast_intersectable_list>
{
    ast_intersectable_list operator()(ast_intersectable_list l)
    {
        ast_intersectable_list out;
        BOOST_FOREACH(ast_intersectable i, l.children)
        {
            ast_intersectable_list current = boost::apply_visitor(*this, i);
            out.children.insert(out.children.end(), current.children.begin(), current.children.end());
        }
        return out;
    }

    template<typename T>
    ast_intersectable_list operator()(T t) const
    {
        ast_intersectable_list l;
        l.children.push_back(t);
        return l;
    }
};

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

template<typename T> bool operator==(const T & lhs, const T & rhs) {
    return boost::fusion::equal_to(lhs, rhs);
}

struct variant_hasher: public boost::static_visitor<std::size_t> {
    template <class T>
    std::size_t operator()(T const& val) const {
        using namespace boost;
        boost::hash<T> hasher;
        return hasher(val);
    }
};

struct hash_combine_s {
    typedef std::size_t result_type;
    template<class T>
    std::size_t operator()(std::size_t current, const T& arg) {
        boost::hash_combine(current, arg);
        return(current);
    }
};

template<typename T>
std::size_t hash_value(const T& t)
{
    return boost::fusion::fold(t, 0, hash_combine_s());
}

#if BOOST_VERSION < 105000
template < BOOST_VARIANT_ENUM_PARAMS(typename T) >
std::size_t hash_value(boost::variant< BOOST_VARIANT_ENUM_PARAMS(T) > const& val) {
    std::size_t seed = boost::apply_visitor(variant_hasher(), val);
    boost::hash_combine(seed, val.which());
    return seed;
}
#endif

typedef std::unordered_map<ast_literal_material, Material*, variant_hasher> ast_mat_map;

struct material_builder : boost::static_visitor<Material*>
{
  material_builder(std::map<std::string, Material*>& materials, ast_mat_map &ast_materials) : ast_materials(ast_materials), materials(materials) {}

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

  Material* operator()(ast_scaled_texture_material material) const
  {
    TextureMaterial* out = new TextureMaterial(material.coefficient.asSpectrum(), material.gamma);
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
          std::cerr << "No such variable: " << name << std::endl;
          return DarkMatter::getInstance();
      }
      else
      {
          return material;
      }
  }

  Material* operator()(ast_literal_material material) const
  {
      if(!ast_materials.count(material))
      {
          ast_materials[material] = boost::apply_visitor(*this, material);
      }
      return ast_materials[material];
  }

  ast_mat_map &ast_materials;
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

struct csg_builder : boost::static_visitor<CSGObject*>
{
    csg_builder(std::map<std::string, Material*>& materials, ast_mat_map &ast_materials, bool buildMaterials = true) : ast_materials(ast_materials), material_b(materials, ast_materials), buildMaterials(buildMaterials) {}

    inline Material * getMaterial(ast_material m)
    {
        if(buildMaterials)
        {
            return boost::apply_visitor(material_b, m);
        }
        else
        {
            return DarkMatter::getInstance();
        }
    }

    Sphere* operator()(const ast_sphere& s)
    {
      return new Sphere(s.center.asQVector(), s.radius, getMaterial(s.material));
    }

    AxisAlignedBox* operator()(const ast_box& b)
    {
      return new AxisAlignedBox(b.min.asQVector(), b.max.asQVector(), getMaterial(b.material));
    }

    CSGIntersection* operator()(const ast_csg_isect& c);
    CSGUnion* operator()(const ast_csg_union& c);
    CSGDifference* operator()(const ast_csg_difference& c);
    CSGXor* operator()(const ast_csg_xor& c);

private:
    ast_mat_map &ast_materials;

    material_builder material_b;

    const bool buildMaterials;
};

CSGIntersection* csg_builder::operator ()(const ast_csg_isect& c)
{
    return new CSGIntersection(boost::apply_visitor(*this, c.left), boost::apply_visitor(*this, c.right));
}

CSGUnion* csg_builder::operator ()(const ast_csg_union& c)
{
    return new CSGUnion(boost::apply_visitor(*this, c.left), boost::apply_visitor(*this, c.right));
}

CSGDifference* csg_builder::operator ()(const ast_csg_difference& c)
{
    return new CSGDifference(boost::apply_visitor(*this, c.left), boost::apply_visitor(*this, c.right));
}

CSGXor* csg_builder::operator ()(const ast_csg_xor& c)
{
    return new CSGXor(boost::apply_visitor(*this, c.left), boost::apply_visitor(*this, c.right));
}

struct intersectable_builder : boost::static_visitor<Intersectable*>
{
  intersectable_builder(std::map<std::string, Material*>& materials, ast_mat_map &ast_materials, bool buildMaterials = true) :
    ast_materials(ast_materials), material_b(materials, ast_materials),
    csg_b(materials, ast_materials, buildMaterials), buildMaterials(buildMaterials)
        {}

  inline Material * getMaterial(ast_material m)
  {
      if(buildMaterials)
      {
          return boost::apply_visitor(material_b, m);
      }
      else
      {
          return DarkMatter::getInstance();
      }
  }

  Quad* operator()(const ast_quad& q)
  {
    return new Quad(q.p1.asQVector(), q.p2.asQVector(), q.p3.asQVector(), q.p4.asQVector(), getMaterial(q.material));
  }

  Plane* operator()(const ast_plane& p)
  {
      return new Plane(p.vector.asQVector(), getMaterial(p.material));
  }

  Triangle* operator()(const ast_triangle& t)
  {
      if(t.n1.asQVector().isNull() || t.n2.asQVector().isNull() || t.n3.asQVector().isNull())
      {
          return new Triangle(t.p1.asQVector(), t.p2.asQVector(), t.p3.asQVector(), t.t1.asCVPoint(), t.t2.asCVPoint(), t.t3.asCVPoint(), getMaterial(t.material));
      }
      else
      {
        return new Triangle(t.p1.asQVector(), t.p2.asQVector(), t.p3.asQVector(), t.n1.asQVector(), t.n2.asQVector(), t.n3.asQVector(), t.t1.asCVPoint(), t.t2.asCVPoint(), t.t3.asCVPoint(), getMaterial(t.material));
      }
  }

  IntersectableList* operator()(const ast_intersectable_list& l);
  IntersectableInstance* operator()(const ast_instance& i);
  BVHNode* operator()(const ast_bvh_node& b);
  Intersectable* operator()(const ast_obj& o);
  CSGObject* operator()(const ast_csg& c);

  ast_mat_map &ast_materials;

  material_builder material_b;
  csg_builder csg_b;

  const bool buildMaterials;
};

IntersectableList* intersectable_builder::operator()(ast_intersectable_list const& l)
{
  vector<Intersectable*> intersectables;
  intersectables.reserve(l.children.size());

  BOOST_FOREACH( ast_intersectable n, l.children )
  {
    intersectables.push_back(boost::apply_visitor(*this, n));
  }

  return new IntersectableList(intersectables);
}

IntersectableInstance* intersectable_builder::operator()(const ast_instance& i)
{
    return new IntersectableInstance(i.transform.asQMatrix4x4(), boost::apply_visitor(*this, i.intersectable));
}

BVHNode* intersectable_builder::operator ()(const ast_bvh_node& b)
{
    Intersectable *left = boost::apply_visitor(*this, b.left), *right = boost::apply_visitor(*this, b.right);
    AxisAlignedBox* bb = (csg_b)(b.bb);
    return new BVHNode(left, right, bb);
}

Intersectable* intersectable_builder::operator()(const ast_obj& o)
{
    ast_diffuse_material darkmatter = {ast_vector3_literal()};
    ast_material mat = buildMaterials ? o.material : darkmatter;
    ObjReader reader;
    ast_intersectable_list inters = reader.load(o.filename.c_str(), mat);
    Intersectable* mesh = (*this)(inters);
    const AxisAlignedBox* bb = mesh->boundingBox();
    QVector3D min = bb->getMin(), max = bb->getMax();
    std::cerr << "Mesh bb min: " << min.x() << "," << min.y() << "," << min.z() << "; max: " << max.x() << "," << max.y() << "," << max.z() << std::endl;
    return mesh;
}

CSGObject* intersectable_builder::operator ()(const ast_csg& c)
{
    return boost::apply_visitor(csg_b, c);
}

struct light_builder : boost::static_visitor<Light*>
{
    Light* operator()(ast_point_light point_light) const
    {
        return new PointLight(point_light.location.asQVector(), point_light.intensity.asSpectrum());
    }

    Light* operator()(ast_area_light area_light) const
    {
        return new AreaLight(area_light.location.asQVector(), area_light.u_direction.asQVector(), area_light.v_direction.asQVector(), area_light.intensity.asSpectrum());
    }

    Light* operator()(ast_cone_light cone_light) const
    {
        return new ConeLight(cone_light.location.asQVector(), cone_light.direction.asQVector(), cone_light.angle, cone_light.intensity.asSpectrum());
    }

    Light* operator()(ast_environment_map envmap) const
    {
        EnvironmentMap *out = new EnvironmentMap(envmap.coefficient.asSpectrum());
        if(out->load(envmap.filename))
        {
            return out;
        }
        else
        {
            delete out;
            return new BlackLight();
        }
    }
};

struct scene_builder : boost::static_visitor<void>
{
    scene_builder() : intersectable_b(materials, ast_materials), material_b(materials, ast_materials) {}

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
        vector<Light*> out;
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
            std::cerr << "Could not assign to " << current_name << ", no such variable: " << identifier << std::endl;
        }
    }

    Scene getScene()
    {
        Scene result(cameras["camera"]);
        std::vector<Intersectable*> intersectablelist;
        intersectablelist.insert(intersectablelist.end(), lights["lights"].begin(), lights["lights"].end());
        intersectablelist.push_back(intersectables["intersectable"]);
        IntersectableList *list = new IntersectableList(intersectablelist);
        Intersectable* intersectable;
        if(vm.count("no-bvh"))
        {
            intersectable = list;
        }
        else
        {
            intersectable = list->createBVH();
        }
        const AxisAlignedBox* bb = intersectable->boundingBox();
        result.object = intersectable;
        QVector3D min = bb->getMin(), max = bb->getMax();
        std::cerr << "Mesh bb min: " << min.x() << "," << min.y() << "," << min.z() << "; max: " << max.x() << "," << max.y() << "," << max.z() << std::endl;
        result.light.insert(result.light.end(), lights["lights"].begin(), lights["lights"].end());
        return result;
    }

    map<string, Camera> cameras;
    map<string, Intersectable*> intersectables;
    map<string, vector<Light*> > lights;
    map<string, Material*> materials;

    ast_mat_map ast_materials;

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
  BOOST_FOREACH(ast_assignment & assignment, assignments)
  {
      builder.addAssignment(assignment);
  }
  return builder.getScene();
}

void resolveVars(vector<ast_assignment> &assignments)
{
    std::cerr << "Loading OBJs..." << std::endl;
    IntersectableAssignmentVisitor<ObjLoader> ol;
    BOOST_FOREACH(ast_assignment & assignment, assignments)
    {
      ol.apply(assignment);
    }

    std::map<std::string, ast_literal_material> objMaterials = ol.getVisitor().getMaterials();
    typedef std::pair<std::string, ast_literal_material> PairType;
    BOOST_FOREACH(PairType material, objMaterials)
    {
        ast_assignment a;
        a.name = material.first;
        a.value = material.second;
        assignments.insert(assignments.begin(), a);
    }
}

vector<ast_assignment> createBVH(vector<ast_assignment> assignments)
{
    std::cerr << "Flattening..." << std::endl;
    IntersectableAssignmentVisitor<Flattener> fl;
    BOOST_FOREACH(ast_assignment & assignment, assignments)
    {
        fl.apply(assignment);
    }

    std::cerr << "Creating BVH tree..." << std::endl;
    IntersectableAssignmentVisitor<BVHCreator> bc;
    BOOST_FOREACH(ast_assignment & assignment, assignments)
    {
      bc.apply(assignment);
    }

    return assignments;
}

const AxisAlignedBox* getBoundingBoxFromAst(ast_intersectable i)
{
    ast_mat_map ast_materials;
    map<std::string, Material*> materials;
    intersectable_builder builder(materials, ast_materials, false);
    Intersectable * intersectable = boost::apply_visitor(builder, i);
    const AxisAlignedBox* bb = intersectable->boundingBox();
    delete intersectable;
    return bb;
}

struct PathVisitor : boost::static_visitor<ast_intersectable>
{
    PathVisitor(QDir dir) : dir(dir) {}

    template<typename T>
    ast_intersectable operator()(T t) const
    {
        return t;
    }

    ast_intersectable operator()(ast_instance i) const
    {
        i.intersectable = boost::apply_visitor(*this, i.intersectable);
        return i;
    }

    ast_intersectable operator()(ast_bvh_node b) const
    {
        b.left = boost::apply_visitor(*this, b.left);
        b.right = boost::apply_visitor(*this, b.right);
        return b;
    }

    ast_intersectable operator()(ast_intersectable_list l) const
    {
        for(unsigned int i = 0; i < l.children.size(); i++)
        {
            l.children[i] = boost::apply_visitor(*this, l.children[i]);
        }
        return l;
    }

    ast_intersectable operator()(ast_obj obj) const
    {
        obj.filename = dir.filePath(QString::fromStdString(obj.filename)).toStdString();
        return obj;
    }

private:
    QDir dir;
};

void applyPath(std::vector<ast_assignment> &assignments, QDir dir)
{
    PathVisitor pv(dir);
    IntersectableAssignmentVisitor<PathVisitor> apv(pv);
    apv.applyAll(assignments);
}
