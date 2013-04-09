#ifndef SCENEDUMPER_H
#define SCENEDUMPER_H

#include "sceneast.h"

#include <boost/variant/static_visitor.hpp>
#include <boost/fusion/include/for_each.hpp>

#include <vector>
#include <iostream>

class SceneDumper
{
public:
    SceneDumper(std::ostream &out = std::cout);
    void dump(std::vector<ast_assignment> assignments);
    void dump(ast_assignment assignment);


private:
    std::ostream &out;
};

#endif // SCENEDUMPER_H
