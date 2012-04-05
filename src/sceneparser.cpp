#include "sceneparser.h"

#include <iostream>

#include <boost/foreach.hpp>

using namespace std;

struct printer;
struct node_printer;

struct printer
{
  void operator()(lst const& l) const;
};

struct node_printer : boost::static_visitor<>
{
  void operator()(const sphere& s) const
  {
    std::cout << s.radius;
    this->operator()(s.center);
  }

  void operator()(const vctor& v) const
  {
    std::cout << "[" << v.x << " " << v.y << " " << v.z << "]";
  }

  void operator()(const lst& l) const
  {
    std::cout << '{';
    printer()(l);
    std::cout << '}';
  }
};

void printer::operator()(lst const& l) const
{
  BOOST_FOREACH( node n, l.children )
  {
    boost::apply_visitor(node_printer(), n);
  }
}

SceneGrammar::SceneGrammar() : SceneGrammar::base_type(nd)
{
  nd %= list | sp;
  list %= boost::spirit::lit("list") >> boost::spirit::lit("{") >> *nd >> boost::spirit::lit("}");
  sp %= boost::spirit::lit("sphere") >> "(" >> vect >> boost::spirit::tag::float_() >> ")";
  vect %= boost::spirit::lit("[") >> boost::spirit::tag::float_() >> boost::spirit::tag::float_() >> boost::spirit::tag::float_() >> "]";
}

bool SceneGrammar::parse(string in)
{
  std::string::iterator begin = in.begin();
  std::string::iterator end = in.end();
  bool r = qi::phrase_parse(begin, end, *this, ast, boost::spirit::ascii::space);
  return r && begin == end;
}
