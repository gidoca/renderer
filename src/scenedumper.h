#ifndef SCENEDUMPER_H
#define SCENEDUMPER_H

#include "sceneast.h"

#include <boost/variant/static_visitor.hpp>

#include <vector>
#include <iostream>

class SceneDumper : public boost::static_visitor<>
{
public:
    SceneDumper(std::ostream &out = std::cout);
    void dump(std::vector<ast_assignment> assignments);
    void dump(ast_assignment assignment);

    void operator()(std::vector<ast_light> value);
    void operator()(ast_camera value);
    void operator()(ast_intersectable value);
    void operator ()(ast_literal_material l);
    void operator()(ast_mirror_material value);
    void operator()(ast_phong_material value);
    void operator()(ast_refractive_material value);
    void operator()(ast_diffuse_material value);
    void operator()(ast_texture_material value);
    void operator()(std::string value);

private:
    std::ostream &out;
};

#endif // SCENEDUMPER_H
