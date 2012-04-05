#include "sceneparser.h"

#include <iostream>

#include <boost/foreach.hpp>

using namespace std;

/*struct printer;
struct node_printer;

struct printer
{
  void operator()(ast_list const& l) const;
};

struct node_printer : boost::static_visitor<>
{
  void operator()(const ast_sphere& s) const
  {
    std::cout << s.radius;
    this->operator()(s.center);
  }

  void operator()(const ast_vector& v) const
  {
    std::cout << "[" << v.x << " " << v.y << " " << v.z << "]";
  }

  void operator()(const ast_list& l) const
  {
    std::cout << '{';
    printer()(l);
    std::cout << '}';
  }
};

void printer::operator()(ast_list const& l) const
{
  BOOST_FOREACH( ast_intersectable n, l.children )
  {
    boost::apply_visitor(node_printer(), n);
  }
}*/

SceneGrammar::SceneGrammar() : SceneGrammar::base_type(intersectable_rule)
{
  intersectable_rule %= list_rule | sphere_rule | box_rule | quad_rule;
  list_rule %= boost::spirit::lit("list") >> boost::spirit::lit("{") >> *intersectable_rule >> boost::spirit::lit("}");
  sphere_rule %= boost::spirit::lit("sphere") >> "(" >> vector_rule >> boost::spirit::tag::float_() >> material_rule >> ")";
  box_rule %= boost::spirit::lit("box") >> "(" >> vector_rule >> vector_rule >> material_rule >> ")";
  quad_rule %= boost::spirit::lit("quad") >> "(" >> vector_rule >> vector_rule >> vector_rule >> vector_rule >> material_rule >> ")";
  vector_rule %= boost::spirit::lit("[") >> boost::spirit::tag::float_() >> boost::spirit::tag::float_() >> boost::spirit::tag::float_() >> "]";
  diffuse_material_rule %= boost::spirit::lit("diffuse") >> "(" >> vector_rule >> ")";
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
