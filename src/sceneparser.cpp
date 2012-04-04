#include "sceneparser.h"

#include <iostream>

#include <boost/foreach.hpp>

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

int main(void)
{
  SceneGrammar s;
  bool r = s.parse("list {list{} list { list {sphere([1 2 3] 4) } } }");
  if(r)
  {
    printer p;
    p(s.getAst());
  }
  else
  {
    std::cout << "Failed to parse string\n";
  }
  return 0;
}

