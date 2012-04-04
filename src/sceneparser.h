#ifndef SCENEPARSER_H
#define SCENEPARSER_H

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/foreach.hpp>

#include <vector>
#include <string>

namespace qi = boost::spirit::qi;

struct lst;
struct sphere;
struct vctor;

typedef boost::variant<
    boost::recursive_wrapper<lst>,
    sphere
    > node;

struct lst
{
  std::vector<node> children;
};

struct vctor
{
  float x, y, z;
};

struct sphere
{
  vctor center;
  float radius;
};

BOOST_FUSION_ADAPT_STRUCT(
    lst,
    (std::vector<node>, children)
)

BOOST_FUSION_ADAPT_STRUCT(
    sphere,
    (vctor, center)
    (float, radius)
)

BOOST_FUSION_ADAPT_STRUCT(
    vctor,
    (float, x)
    (float, y)
    (float, z)
)

struct SceneGrammar : qi::grammar<std::string::iterator, lst(), boost::spirit::ascii::space_type>
{
public:
  SceneGrammar() : SceneGrammar::base_type(list)
  {
    nd %= list | sp;
    list %= boost::spirit::lit("list") >> boost::spirit::lit("{") >> *nd >> boost::spirit::lit("}");
    sp %= boost::spirit::lit("sphere") >> "(" >> vect >> boost::spirit::tag::float_() >> ")";
    vect %= boost::spirit::lit("[") >> boost::spirit::tag::float_() >> boost::spirit::tag::float_() >> boost::spirit::tag::float_() >> "]";
  }

  bool parse(std::string in)
  {
    std::string::iterator begin = in.begin();
    std::string::iterator end = in.end();
    bool r = qi::phrase_parse(begin, end, *this, ast, boost::spirit::ascii::space);
    return r && begin == end;
  }

  lst getAst()
  {
    return ast;
  }

private:
  qi::rule<std::string::iterator, lst(), boost::spirit::ascii::space_type> list;
  qi::rule<std::string::iterator, sphere(), boost::spirit::ascii::space_type> sp;
  qi::rule<std::string::iterator, node(), boost::spirit::ascii::space_type> nd;
  qi::rule<std::string::iterator, vctor(), boost::spirit::ascii::space_type> vect;

  lst ast;
};

#endif // SCENEPARSER_H
