#include "sceneparser.h"

#include <iostream>

#include <boost/foreach.hpp>

using namespace std;

SceneGrammar::SceneGrammar() : SceneGrammar::base_type(intersectable_rule)
{
  intersectable_rule %= list_rule | sphere_rule | box_rule | quad_rule | instance_rule;
  list_rule %= boost::spirit::lit("list") >> boost::spirit::lit("{") >> *intersectable_rule >> boost::spirit::lit("}");
  sphere_rule %= boost::spirit::lit("sphere") >> "(" >> vector_literal_rule >> "," >> boost::spirit::tag::float_() >> "," >> material_rule >> ")";
  box_rule %= boost::spirit::lit("box") >> "(" >> vector_literal_rule >> "," >> vector_literal_rule >> "," >> material_rule >> ")";
  quad_rule %= boost::spirit::lit("quad") >> "(" >> vector_literal_rule >> "," >> vector_literal_rule >> "," >> vector_literal_rule >> "," >> vector_literal_rule >> "," >> material_rule >> ")";
  instance_rule %= boost::spirit::lit("instance") >> "(" >> matrix_rule >> "," >> intersectable_rule >> ")";

  vector_literal_rule %= boost::spirit::lit("[") >> boost::spirit::tag::float_() >> boost::spirit::tag::float_() >> boost::spirit::tag::float_() >> "]";
  matrix_literal_rule %= boost::spirit::lit("[") >> vector_literal_rule >> vector_literal_rule >> vector_literal_rule >> "]";
  matrix_mul_rule %= matrix_literal_rule >> "*" >> matrix_rule;
  matrix_rule %= matrix_mul_rule | matrix_literal_rule;

  diffuse_material_rule %= boost::spirit::lit("diffuse") >> "(" >> vector_literal_rule >> ")";
  mirror_material_rule %= boost::spirit::lit("mirror") >> "(" >> boost::spirit::tag::float_() >> ")";
  material_rule %= diffuse_material_rule | mirror_material_rule;
}

bool SceneGrammar::parse(string in)
{
  std::string::iterator begin = in.begin();
  std::string::iterator end = in.end();
  //Argument order changed
#if BOOST_VERSION >= 104100
  bool r = qi::phrase_parse(begin, end, *this, boost::spirit::ascii::space, ast);
#else
  bool r = qi::phrase_parse(begin, end, *this, ast, boost::spirit::ascii::space);
#endif
  return r && begin == end;
}
