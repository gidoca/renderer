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

struct SceneGrammar : qi::grammar<std::string::iterator, node(), boost::spirit::ascii::space_type>
{
public:
  SceneGrammar();

  bool parse(std::string in);

  node getAst()
  {
    return ast;
  }

private:
  qi::rule<std::string::iterator, lst(), boost::spirit::ascii::space_type> list;
  qi::rule<std::string::iterator, sphere(), boost::spirit::ascii::space_type> sp;
  qi::rule<std::string::iterator, node(), boost::spirit::ascii::space_type> nd;
  qi::rule<std::string::iterator, vctor(), boost::spirit::ascii::space_type> vect;

  node ast;
};

#endif // SCENEPARSER_H
