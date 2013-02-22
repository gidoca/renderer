#include "scenedumper.h"

#include <boost/foreach.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/fusion/include/for_each.hpp>

using boost::fusion::begin;
using boost::fusion::end;
using boost::fusion::next;

#include <iostream>

using namespace std;

class ad
{
public:
    ad(bool *first) : first(first) {}

    void operator()(ast_vector3_literal v) const
    {
        c();
        cout << '[' << v.x << ' ' << v.y << ' ' << v.z << ']';
    }

    void operator()(ast_vector4_literal v) const
    {
        c();
        cout << '[' << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w << ']';
    }

    void operator()(float f) const
    {
        c();
        cout << f;
    }

    void operator()(std::string s) const
    {
        c();
        cout << '"' << s << '"';
    }

private:
    bool * const first;

    void c() const
    {
        if(*first)
        {
            *first = false;
        }
        else
        {
            cout << ',';
        }
    }
};

SceneDumper::SceneDumper(std::ostream &out) : out(out) {}

void SceneDumper::dump(ast_assignment assignment)
{
    out << assignment.name << "=";
    boost::apply_visitor(*this, assignment.value);
    out << ";" << endl;
}

void SceneDumper::dump(vector<ast_assignment> assignments)
{
    BOOST_FOREACH(ast_assignment assignment, assignments) {
        dump(assignment);
    }
}



void SceneDumper::operator()(std::vector<ast_light> value)
{
    out << "lights{" << "}";
}

void SceneDumper::operator()(ast_camera value)
{
    out << "camera(";
//    for(auto i = begin(value); i != end(value); i);
    bool first = true;
    boost::fusion::for_each(value, ad(&first));

    out << ")";
}

void SceneDumper::operator()(ast_intersectable value)
{

}

void SceneDumper::operator ()(ast_literal_material l)
{
    boost::apply_visitor(*this, l);
}

void SceneDumper::operator()(ast_diffuse_material value)
{
    out << "diffuse(";
    bool first = true;
    boost::fusion::for_each(value, ad(&first));
    out << ')';
}

void SceneDumper::operator()(ast_mirror_material value)
{
    out << "mirror(";
    bool first = true;
    boost::fusion::for_each(value, ad(&first));
    out << ')';
}

void SceneDumper::operator()(ast_phong_material value)
{
    out << "phong(";
    bool first = true;
    boost::fusion::for_each(value, ad(&first));
    out << ')';
}

void SceneDumper::operator()(ast_refractive_material value)
{
    out << "refractive(";
    bool first = true;
    boost::fusion::for_each(value, ad(&first));
    out << ')';
}

void SceneDumper::operator()(ast_texture_material value)
{
    out << "texture(";
    bool first = true;
    boost::fusion::for_each(value, ad(&first));
    out << ')';
}

void SceneDumper::operator()(std::string value)
{
    out << value;
}

