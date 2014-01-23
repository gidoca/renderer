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
#include "sceneparser.h"

#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>

#include <boost/foreach.hpp>
#include <boost/variant.hpp>

using namespace std;

SceneGrammar::SceneGrammar() : SceneGrammar::base_type(assignments_rule, "intersectable")
{
  using boost::phoenix::construct;
  using boost::phoenix::val;
  using namespace boost::spirit;

  string_literal_rule %= lit('"') >> *(ascii::char_ - '"') >> '"';
  identifier_rule %= (ascii::alpha | '_') >> *(ascii::alnum | '_');

  assignments_rule %= *assignment_rule;
  assignment_rule %= identifier_rule >> "=" >> value_rule >> ";";
  value_rule %= intersectable_rule | light_list_rule | camera_rule | material_literal_rule | identifier_rule;

  intersectable_rule %= intersectable_list_rule | quad_rule | plane_rule | obj_rule | triangle_rule | instance_rule | bvh_node_rule | csg_rule;
  intersectable_rule.name("intersectable");
  intersectable_list_rule %= lit("intersectables") >> lit("{") >> *intersectable_rule >> lit("}");
  intersectable_list_rule.name("list of intersectables");
  sphere_rule %= lit("sphere") >> "(" >> vector3_literal_rule >> "," >> tag::float_() >> "," >> material_rule >> ")";
  sphere_rule.name("sphere");
  box_rule %= lit("box") >> "(" >> vector3_literal_rule >> "," >> vector3_literal_rule >> "," >> material_rule >> ")";
  box_rule.name("box");
  csg_rule %= csg_isect_rule | csg_union_rule | csg_difference_rule | csg_xor_rule | sphere_rule | box_rule;
  csg_rule.name("csg");
  csg_isect_rule %= lit("isect") >> "(" >> csg_rule >> "," >> csg_rule >> ")";
  csg_isect_rule.name("csg intersection");
  csg_union_rule %= lit("union") >> "(" >> csg_rule >> "," >> csg_rule >> ")";
  csg_union_rule.name("csg union");
  csg_difference_rule %= lit("diff") >> "(" >> csg_rule >> "," >> csg_rule >> ")";
  csg_difference_rule.name("csg difference");
  csg_xor_rule %= lit("xor") >> "(" >> csg_rule >> "," >> csg_rule >> ")";
  csg_xor_rule.name("csg xor");
  quad_rule %= lit("quad") >> "(" >> vector3_literal_rule >> "," >> vector3_literal_rule >> "," >> vector3_literal_rule >> "," >> vector3_literal_rule >> "," >> material_rule >> ")";
  quad_rule.name("quad");
  plane_rule %= lit("plane") >> "(" >> vector4_literal_rule >> "," >> material_rule >> ")";
  plane_rule.name("plane");
  obj_rule %= lit("obj") >> "(" >> string_literal_rule >> "," >> material_rule >> ")";
  obj_rule.name("obj");
  triangle_rule %= lit("t") >> "(" >> vector3_literal_rule >> "," >> vector3_literal_rule >> "," >> vector3_literal_rule >> "," >> vector3_literal_rule >> "," >> vector3_literal_rule >> "," >> vector3_literal_rule >> "," >> vector2_literal_rule >> "," >> vector2_literal_rule >> "," >> vector2_literal_rule >> "," >> material_rule >> ")";
  triangle_rule.name("triangle");
  instance_rule %= lit("instance") >> "(" >> matrix_rule >> "," >> intersectable_rule >> ")";
  instance_rule.name("instance");
  bvh_node_rule %= lit("b") >> "(" >> intersectable_rule >> "," >> intersectable_rule >> "," >> box_rule >> ")";
  bvh_node_rule.name("bvh node");

  vector2_literal_rule %= lit("[") >> tag::float_() >> tag::float_() >> "]";
  vector2_literal_rule.name("2-vector literal");
  vector3_literal_rule %= lit("[") >> tag::float_() >> tag::float_() >> tag::float_() >> "]";
  vector3_literal_rule.name("3-vector literal");
  vector4_literal_rule %= lit("[") >> tag::float_() >> tag::float_() >> tag::float_() >> tag::float_() >> "]";
  vector4_literal_rule.name("4-vector literal");
  matrix_literal_rule %= lit("[") >> vector4_literal_rule >> vector4_literal_rule >> vector4_literal_rule >> vector4_literal_rule >> "]";
  matrix_literal_rule.name("matrix literal");
  matrix_translate_rule %= lit("translate") >> "(" >> vector3_literal_rule >> ")";
  matrix_translate_rule.name("translation matrix");
  matrix_rotate_rule %= lit("rotate") >> "(" >> tag::float_() >> "," >> vector3_literal_rule >> ")";
  matrix_rotate_rule.name("rotation matrix");
  matrix_scale_rule %= lit("scale") >> "(" >> tag::float_() >>  ")";
  matrix_scale_rule.name("scale matrix");
  matrix_scale_vect_rule %= lit("scale") >> "(" >> vector3_literal_rule >>  ")";
  matrix_scale_vect_rule.name("scale matrix non-uniformely");
  matrix_rule %= basic_matrix_rule >> *(lit("*") >> basic_matrix_rule);
  matrix_rule.name("matrix");
  basic_matrix_rule %= matrix_literal_rule | matrix_translate_rule | matrix_rotate_rule | matrix_scale_rule | matrix_scale_vect_rule;
  basic_matrix_rule.name("basic matrix");

  diffuse_material_rule %= lit("diffuse") >> "(" >> vector3_literal_rule >> ")";
  diffuse_material_rule.name("diffuse material");
  phong_material_rule %= lit("phong") >> "(" >> vector3_literal_rule >> "," >> vector3_literal_rule >> "," >> tag::float_() >> ")";
  phong_material_rule.name("phong material");
  blinn_material_rule %= lit("blinn") >> "(" >> vector3_literal_rule >> "," >> vector3_literal_rule >> "," >> tag::float_() >> ")";
  blinn_material_rule.name("blinn material");
  mirror_material_rule %= lit("mirror") >> "(" >> tag::float_() >> ")";
  mirror_material_rule.name("mirror material");
  texture_material_rule %= lit("texture") >> "(" >> string_literal_rule >> "," >> vector3_literal_rule >> "," >> tag::float_() >> ")";
  texture_material_rule.name("scaled texture material");
  refractive_material_rule %= lit("refractive") >> "(" >> tag::float_() >> ")";
  refractive_material_rule.name("refractive material");
  material_rule %= material_literal_rule | identifier_rule;
  material_rule.name("material");
  material_literal_rule %= diffuse_material_rule | phong_material_rule | blinn_material_rule | mirror_material_rule | texture_material_rule | refractive_material_rule;
  material_literal_rule.name("material literal");

  camera_rule %= lit("camera") >> "(" >> vector3_literal_rule >> "," >> vector3_literal_rule >> "," >> vector3_literal_rule >> "," >> tag::float_() >> "," >> tag::float_() >> "," >> tag::float_() >> ")";

  light_list_rule %= lit("lights") >> "{" >> *light_rule >> "}";
  point_light_rule %= lit("pointlight") >> "(" >> vector3_literal_rule >> "," >> vector3_literal_rule >> ")";
  area_light_rule %= lit("arealight") >> "(" >> vector3_literal_rule >> "," >> vector3_literal_rule >> "," >> vector3_literal_rule >> "," >> vector3_literal_rule >> ")";
  cone_light_rule %= lit("conelight") >> "(" >> vector3_literal_rule >> "," >> vector3_literal_rule >> "," >> tag::float_() >> "," >> vector3_literal_rule >> ")";
  environment_map_rule %= lit("environmentmap") >> "(" >> string_literal_rule >> "," >> vector3_literal_rule >> ")";
  light_rule %= point_light_rule | area_light_rule | cone_light_rule | environment_map_rule;

  boost::spirit::qi::on_error<boost::spirit::qi::fail>
  (
      assignments_rule,
      cerr << val("Error, expecting ") << _4 << val(" here: \"") << construct<std::string>(_3, _2) << val("\"\n") << std::endl
  );
}

bool SceneGrammar::parse(string filename)
{

  fileDir = QFileInfo(QString::fromStdString(filename)).dir();

  ifstream in(filename.c_str(), ifstream::in);
  if(!in.is_open() || in.eof())
  {
      std::cerr << "Failed to open file " << filename << std::endl;
      return false;
  }
  std::string str((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
  std::string::iterator begin = str.begin();
  std::string::iterator end = str.end();
  //istream_iterator doesn't seem to work for some reason
  /*boost::spirit::istream_iterator begin(in);
  boost::spirit::istream_iterator end;*/

  //Argument order changed
  bool r = qi::phrase_parse(begin, end, assignments_rule, skipper, ast);
  applyPath(ast, fileDir);
  return r && begin == end;
}
