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
#ifndef GLOBAL_H
#define GLOBAL_H

class AreaLight;
class AxisAlignedBox;
class BiDiPathTracingIntegrator;
class BinaryMaterial;
class BVHNode;
class Camera;
class Conelight;
class CSGIntersection;
class CSGObject;
class DarkMatter;
class DiffuseMaterial;
class EnergyRedistributionRenderer;
class HitRecord;
class Integrator;
class Intersectable;
class IntersectableInstance;
class IntersectableList;
class IntersectionParamter;
class JitteredSampler;
class Light;
class Material;
class MetropolisRenderer;
class MetropolisFltRenderer;
class MetropolisSample;
class MirrorMaterial;
class ObjReader;
class Path;
class PerPixelRenderer;
class PhongMaterial;
class Plane;
class PointLight;
class Quad;
class Ray;
class Renderer;
class Sample;
class Sampler;
class Scene;
class SceneGrammar;
class SimpleIntegrator;
class Sphere;
class Tonemapper;
class TransparentMaterial;
class Triangle;
class UniDiPathTracingIntegrator;

struct ast_vector3_literal;
struct ast_vector4_literal;
struct ast_diffuse_material;
struct ast_phong_material;
struct ast_texture_material;
struct ast_refractive_material;
struct ast_mirror_material;
struct ast_matrix_literal;
struct ast_matrix_translate;
struct ast_matrix_rotate;
struct ast_matrix_scale;
struct ast_matrix_scale_vect;
struct ast_matrix;
struct ast_intersectable_list;
struct ast_sphere;
struct ast_box;
struct ast_quad;
struct ast_plane;
struct ast_obj;
struct ast_triangle;
struct ast_instance;
struct ast_bvh_node;
struct ast_camera;
struct ast_point_light;
struct ast_area_light;
struct ast_cone_light;
struct ast_intersectable_assignment;
struct ast_camera_assignment;
struct ast_light_assignment;

#endif // GLOBAL_H
