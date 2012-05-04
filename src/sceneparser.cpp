#include "sceneparser.h"

#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>

#include <boost/foreach.hpp>

using namespace std;

SceneGrammar::SceneGrammar() : SceneGrammar::base_type(intersectable_rule, "intersectable")
{
  using boost::phoenix::construct;
  using boost::phoenix::val;
  using namespace boost::spirit;

  intersectable_rule %= list_rule | sphere_rule | box_rule | quad_rule | instance_rule;
  intersectable_rule.name("intersectable");
  list_rule %= boost::spirit::lit("list") >> boost::spirit::lit("{") >> *intersectable_rule >> boost::spirit::lit("}");
  list_rule.name("list");
  sphere_rule %= boost::spirit::lit("sphere") >> "(" >> vector3_literal_rule >> "," >> boost::spirit::tag::float_() >> "," >> material_rule >> ")";
  sphere_rule.name("sphere");
  box_rule %= boost::spirit::lit("box") >> "(" >> vector3_literal_rule >> "," >> vector3_literal_rule >> "," >> material_rule >> ")";
  box_rule.name("box");
  quad_rule %= boost::spirit::lit("quad") >> "(" >> vector3_literal_rule >> "," >> vector3_literal_rule >> "," >> vector3_literal_rule >> "," >> vector3_literal_rule >> "," >> material_rule >> ")";
  quad_rule.name("quad");
  instance_rule %= boost::spirit::lit("instance") >> "(" >> matrix_rule >> "," >> intersectable_rule >> ")";
  instance_rule.name("instance");

  vector3_literal_rule %= boost::spirit::lit("[") >> boost::spirit::tag::float_() >> boost::spirit::tag::float_() >> boost::spirit::tag::float_() >> "]";
  vector3_literal_rule.name("3-vector literal");
  vector4_literal_rule %= boost::spirit::lit("[") >> boost::spirit::tag::float_() >> boost::spirit::tag::float_() >> boost::spirit::tag::float_() >> boost::spirit::tag::float_() >> "]";
  vector4_literal_rule.name("4-vector literal");
  matrix_literal_rule %= boost::spirit::lit("[") >> vector4_literal_rule >> vector4_literal_rule >> vector4_literal_rule >> vector4_literal_rule >> "]";
  matrix_literal_rule.name("matrix literal");
  matrix_translate_rule %= boost::spirit::lit("translate") >> "(" >> vector3_literal_rule >> ")";
  matrix_translate_rule.name("translation matrix");
  matrix_rotate_rule %= boost::spirit::lit("rotate") >> "(" >> boost::spirit::tag::float_() >> "," >> vector3_literal_rule >> ")";
  matrix_rotate_rule.name("rotation matrix");
  matrix_rule %= basic_matrix_rule >> *(boost::spirit::lit("*") >> basic_matrix_rule);
  matrix_rule.name("matrix");
  basic_matrix_rule %= matrix_literal_rule | matrix_translate_rule | matrix_rotate_rule;
  basic_matrix_rule.name("basic matrix");

  diffuse_material_rule %= boost::spirit::lit("diffuse") >> "(" >> vector3_literal_rule >> ")";
  diffuse_material_rule.name("diffuse material");
  mirror_material_rule %= boost::spirit::lit("mirror") >> "(" >> boost::spirit::tag::float_() >> ")";
  mirror_material_rule.name("mirror material");
  material_rule %= diffuse_material_rule | mirror_material_rule;
  material_rule.name("material");

  boost::spirit::qi::on_error
  (
      intersectable_rule,
      cerr << val("Error, expecting ") << _4 << val(" here: \"") << construct<std::string>(_3, _2) << val("\"\n")
  );
}

bool SceneGrammar::parse(string filename)
{
  ifstream in(filename.c_str(), ifstream::in);
  std::string str((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
  std::string::iterator begin = str.begin();
  std::string::iterator end = str.end();
  //istream_iterator doesn't seem to work for some reason
  /*boost::spirit::istream_iterator begin(in);
  boost::spirit::istream_iterator end;*/

  //Argument order changed
#if BOOST_VERSION >= 104100
  bool r = qi::phrase_parse(begin, end, *this, boost::spirit::ascii::space, ast);
#else
  bool r = qi::phrase_parse(begin, end, *this, ast, boost::spirit::ascii::space);
#endif
  return r && begin == end;
}