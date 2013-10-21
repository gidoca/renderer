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
#ifndef SCENEPARSER_H
#define SCENEPARSER_H

#include "global.h"

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

#include <vector>
#include <string>

#include "sceneast.h"

namespace qi = boost::spirit::qi;

typedef std::string::iterator it;

struct comment_skipper : public qi::grammar<it> {

    comment_skipper() : comment_skipper::base_type(skip, "Comment") {
        skip = boost::spirit::ascii::space | ("/*" >> *(qi::char_ - "*/") >> "*/") | ("//" >> *(qi::char_ - "\n") >> "\n");
    }
    qi::rule<it> skip;
};

struct SceneGrammar : qi::grammar<it, std::vector<ast_assignment>(), comment_skipper>
{
public:
  SceneGrammar();

  bool parse(std::string filename);

  std::vector<ast_assignment> getAst()
  {
    return ast;
  }

private:
  qi::rule<it, std::vector<char>()> string_literal_rule;
  qi::rule<it, std::vector<char>()> identifier_rule;

  qi::rule<it, std::vector<ast_assignment>(), comment_skipper> assignments_rule;
  qi::rule<it, ast_assignment(), comment_skipper> assignment_rule;
  qi::rule<it, ast_value(), comment_skipper> value_rule;

  qi::rule<it, ast_intersectable_list(), comment_skipper> intersectable_list_rule;
  qi::rule<it, ast_sphere(), comment_skipper> sphere_rule;
  qi::rule<it, ast_box(), comment_skipper> box_rule;
  qi::rule<it, ast_csg(), comment_skipper> csg_rule;
  qi::rule<it, ast_csg_isect(), comment_skipper> csg_isect_rule;
  qi::rule<it, ast_csg_union(), comment_skipper> csg_union_rule;
  qi::rule<it, ast_csg_difference(), comment_skipper> csg_difference_rule;
  qi::rule<it, ast_csg_xor(), comment_skipper> csg_xor_rule;
  qi::rule<it, ast_quad(), comment_skipper> quad_rule;
  qi::rule<it, ast_plane(), comment_skipper> plane_rule;
  qi::rule<it, ast_obj(), comment_skipper> obj_rule;
  qi::rule<it, ast_triangle(), comment_skipper> triangle_rule;
  qi::rule<it, ast_instance(), comment_skipper> instance_rule;
  qi::rule<it, ast_bvh_node(), comment_skipper> bvh_node_rule;
  qi::rule<it, ast_intersectable(), comment_skipper> intersectable_rule;

  qi::rule<it, ast_vector2_literal(), comment_skipper> vector2_literal_rule;
  qi::rule<it, ast_vector3_literal(), comment_skipper> vector3_literal_rule;
  qi::rule<it, ast_vector4_literal(), comment_skipper> vector4_literal_rule;
  qi::rule<it, ast_matrix_literal(), comment_skipper> matrix_literal_rule;
  qi::rule<it, ast_matrix_translate(), comment_skipper> matrix_translate_rule;
  qi::rule<it, ast_matrix_rotate(), comment_skipper> matrix_rotate_rule;
  qi::rule<it, ast_matrix_scale(), comment_skipper> matrix_scale_rule;
  qi::rule<it, ast_matrix_scale_vect(), comment_skipper> matrix_scale_vect_rule;
  qi::rule<it, ast_basic_matrix(), comment_skipper> basic_matrix_rule;
  qi::rule<it, ast_matrix(), comment_skipper> matrix_rule;

  qi::rule<it, ast_diffuse_material(), comment_skipper> diffuse_material_rule;
  qi::rule<it, ast_phong_material(), comment_skipper> phong_material_rule;
  qi::rule<it, ast_mirror_material(), comment_skipper> mirror_material_rule;
  qi::rule<it, ast_texture_material(), comment_skipper> texture_material_rule;
  qi::rule<it, ast_scaled_texture_material(), comment_skipper> scaled_texture_material_rule;
  qi::rule<it, ast_refractive_material(), comment_skipper> refractive_material_rule;
  qi::rule<it, ast_literal_material(), comment_skipper> material_literal_rule;
  qi::rule<it, ast_material(), comment_skipper> material_rule;

  qi::rule<it, ast_camera(), comment_skipper> camera_rule;

  qi::rule<it, std::vector<ast_light>(), comment_skipper> light_list_rule;
  qi::rule<it, ast_point_light(), comment_skipper> point_light_rule;
  qi::rule<it, ast_area_light(), comment_skipper> area_light_rule;
  qi::rule<it, ast_cone_light(), comment_skipper> cone_light_rule;
  qi::rule<it, ast_environment_map(), comment_skipper> environment_map_rule;
  qi::rule<it, ast_light(), comment_skipper> light_rule;

  comment_skipper skipper;

  std::vector<ast_assignment> ast;
};

#endif // SCENEPARSER_H
