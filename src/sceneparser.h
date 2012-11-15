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

struct SceneGrammar : qi::grammar<std::string::iterator, std::vector<ast_assignment>(), boost::spirit::ascii::space_type>
{
public:
  SceneGrammar();

  bool parse(std::string filename);

  std::vector<ast_assignment> getAst()
  {
    return ast;
  }

private:
  qi::rule<std::string::iterator, std::vector<char>() > string_literal_rule;
  qi::rule<std::string::iterator, std::vector<char>(), boost::spirit::ascii::space_type> identifier_rule;

  qi::rule<std::string::iterator, std::vector<ast_assignment>(), boost::spirit::ascii::space_type> assignments_rule;
  qi::rule<std::string::iterator, ast_assignment(), boost::spirit::ascii::space_type> assignment_rule;
  qi::rule<std::string::iterator, ast_value(), boost::spirit::ascii::space_type> value_rule;

  qi::rule<std::string::iterator, ast_intersectable_list(), boost::spirit::ascii::space_type> intersectable_list_rule;
  qi::rule<std::string::iterator, ast_sphere(), boost::spirit::ascii::space_type> sphere_rule;
  qi::rule<std::string::iterator, ast_box(), boost::spirit::ascii::space_type> box_rule;
  qi::rule<std::string::iterator, ast_quad(), boost::spirit::ascii::space_type> quad_rule;
  qi::rule<std::string::iterator, ast_plane(), boost::spirit::ascii::space_type> plane_rule;
  qi::rule<std::string::iterator, ast_obj(), boost::spirit::ascii::space_type> obj_rule;
  qi::rule<std::string::iterator, ast_instance(), boost::spirit::ascii::space_type> instance_rule;
  qi::rule<std::string::iterator, ast_intersectable(), boost::spirit::ascii::space_type> intersectable_rule;

  qi::rule<std::string::iterator, ast_vector3_literal(), boost::spirit::ascii::space_type> vector3_literal_rule;
  qi::rule<std::string::iterator, ast_vector4_literal(), boost::spirit::ascii::space_type> vector4_literal_rule;
  qi::rule<std::string::iterator, ast_matrix_literal(), boost::spirit::ascii::space_type> matrix_literal_rule;
  qi::rule<std::string::iterator, ast_matrix_translate(), boost::spirit::ascii::space_type> matrix_translate_rule;
  qi::rule<std::string::iterator, ast_matrix_rotate(), boost::spirit::ascii::space_type> matrix_rotate_rule;
  qi::rule<std::string::iterator, ast_matrix_scale(), boost::spirit::ascii::space_type> matrix_scale_rule;
  qi::rule<std::string::iterator, ast_basic_matrix(), boost::spirit::ascii::space_type> basic_matrix_rule;
  qi::rule<std::string::iterator, ast_matrix(), boost::spirit::ascii::space_type> matrix_rule;

  qi::rule<std::string::iterator, ast_diffuse_material(), boost::spirit::ascii::space_type> diffuse_material_rule;
  qi::rule<std::string::iterator, ast_mirror_material(), boost::spirit::ascii::space_type> mirror_material_rule;
  qi::rule<std::string::iterator, ast_texture_material(), boost::spirit::ascii::space_type> texture_material_rule;
  qi::rule<std::string::iterator, ast_refractive_material(), boost::spirit::ascii::space_type> refractive_material_rule;
  qi::rule<std::string::iterator, ast_material(), boost::spirit::ascii::space_type> material_rule;

  qi::rule<std::string::iterator, ast_camera(), boost::spirit::ascii::space_type> camera_rule;

  qi::rule<std::string::iterator, std::vector<ast_light>(), boost::spirit::ascii::space_type> light_list_rule;
  qi::rule<std::string::iterator, ast_point_light(), boost::spirit::ascii::space_type> point_light_rule;
  qi::rule<std::string::iterator, ast_area_light(), boost::spirit::ascii::space_type> area_light_rule;
  qi::rule<std::string::iterator, ast_cone_light(), boost::spirit::ascii::space_type> cone_light_rule;
  qi::rule<std::string::iterator, ast_light(), boost::spirit::ascii::space_type> light_rule;

  std::vector<ast_assignment> ast;
};

#endif // SCENEPARSER_H
