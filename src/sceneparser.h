#ifndef SCENEPARSER_H
#define SCENEPARSER_H

#include "global.h"

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <vector>
#include <string>

#include "sceneast.h"

namespace qi = boost::spirit::qi;

struct SceneGrammar : qi::grammar<std::string::iterator, ast_intersectable(), boost::spirit::ascii::space_type>
{
public:
  SceneGrammar();

  bool parse(std::string in);

  ast_intersectable getAst()
  {
    return ast;
  }

private:
  qi::rule<std::string::iterator, ast_list(), boost::spirit::ascii::space_type> list_rule;
  qi::rule<std::string::iterator, ast_sphere(), boost::spirit::ascii::space_type> sphere_rule;
  qi::rule<std::string::iterator, ast_box(), boost::spirit::ascii::space_type> box_rule;
  qi::rule<std::string::iterator, ast_quad(), boost::spirit::ascii::space_type> quad_rule;
  qi::rule<std::string::iterator, ast_instance(), boost::spirit::ascii::space_type> instance_rule;
  qi::rule<std::string::iterator, ast_intersectable(), boost::spirit::ascii::space_type> intersectable_rule;

  qi::rule<std::string::iterator, ast_vector3_literal(), boost::spirit::ascii::space_type> vector3_literal_rule;
  qi::rule<std::string::iterator, ast_vector4_literal(), boost::spirit::ascii::space_type> vector4_literal_rule;
  qi::rule<std::string::iterator, ast_matrix_literal(), boost::spirit::ascii::space_type> matrix_literal_rule;
  qi::rule<std::string::iterator, ast_matrix_mul(), boost::spirit::ascii::space_type> matrix_mul_rule;
  qi::rule<std::string::iterator, ast_matrix_translate(), boost::spirit::ascii::space_type> matrix_translate_rule;
  qi::rule<std::string::iterator, ast_matrix(), boost::spirit::ascii::space_type> matrix_rule;

  qi::rule<std::string::iterator, ast_diffuse_material(), boost::spirit::ascii::space_type> diffuse_material_rule;
  qi::rule<std::string::iterator, ast_mirror_material(), boost::spirit::ascii::space_type> mirror_material_rule;
  qi::rule<std::string::iterator, ast_material(), boost::spirit::ascii::space_type> material_rule;

  ast_intersectable ast;
};

#endif // SCENEPARSER_H
