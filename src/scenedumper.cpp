#include "scenedumper.h"

#include <boost/foreach.hpp>
#include <boost/variant/apply_visitor.hpp>

#include <iostream>

using namespace std;

struct AstVisitor;
class ad;

class MatrixPrinter : public boost::static_visitor<>
{
public:
    MatrixPrinter(std::ostream & out) : out(out) {}

    void operator()(ast_matrix_literal m) const
    {
        out << '[';
        out << '[' << m.v1.x << ' ' << m.v1.y << ' ' << m.v1.z << ' ' << m.v1.w << ']';
        out << '[' << m.v2.x << ' ' << m.v2.y << ' ' << m.v2.z << ' ' << m.v2.w << ']';
        out << '[' << m.v3.x << ' ' << m.v3.y << ' ' << m.v3.z << ' ' << m.v3.w << ']';
        out << '[' << m.v4.x << ' ' << m.v4.y << ' ' << m.v4.z << ' ' << m.v4.w << ']';
        out << ']';
    }

    template<typename T>
    void operator()(T value) const
    {
        out << T::function_name;
        out << "(";
        bool first = true;
        boost::fusion::for_each(value, ad(&first, out));
        out << ')';
    }

private:
    std::ostream & out;
};

class ad
{
public:
    ad(bool *first, std::ostream & out) : first(first), out(out) {}

    void operator()(ast_vector2_literal v) const
    {
        c();
        out << '[' << v.x << ' ' << v.y << ']';
    }

    void operator()(ast_vector3_literal v) const
    {
        c();
        out << '[' << v.x << ' ' << v.y << ' ' << v.z << ']';
    }

    void operator()(ast_vector4_literal v) const
    {
        c();
        out << '[' << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w << ']';
    }

    void operator()(float f) const
    {
        c();
        out << f;
    }

    void operator()(std::string s) const
    {
        c();
        out << '"' << s << '"';
    }

    void operator()(ast_material m) const
    {
    }

    void operator()(ast_matrix m) const
    {
        boost::apply_visitor(MatrixPrinter(out), m.first);
        BOOST_FOREACH(ast_basic_matrix mat, m.mult)
        {
            out << '*';
            boost::apply_visitor(MatrixPrinter(out), mat);
        }
    }

    void operator()(ast_intersectable) const
    {

    }

private:
    bool * const first;
    std::ostream & out;

    void c() const
    {
        if(*first)
        {
            *first = false;
        }
        else
        {
            out << ',';
        }
    }
};

class AstVisitor : public boost::static_visitor<>
{
public:
    AstVisitor(std::ostream & out) : out(out) {}

    void operator()(std::vector<ast_light> value) const;
    void operator()(ast_intersectable value) const;
    void operator()(ast_literal_material l) const;
    void operator()(std::string value) const;
    void operator()(ast_intersectable_list value) const;

    template<typename T>
    void operator()(T value) const
    {
        out << T::function_name;
        out << "(";
        bool first = true;
        boost::fusion::for_each(value, ad(&first, out));
        out << ')';
    }
private:
    std::ostream & out;
};

SceneDumper::SceneDumper(std::ostream &out) : out(out) {}

void SceneDumper::dump(ast_assignment assignment)
{
    out << assignment.name << "=";
    boost::apply_visitor(AstVisitor(out), assignment.value);
    out << ";" << endl;
}

void SceneDumper::dump(vector<ast_assignment> assignments)
{
    BOOST_FOREACH(ast_assignment assignment, assignments) {
        dump(assignment);
    }
}


void AstVisitor::operator()(std::vector<ast_light> value) const
{
    out << "lights{";
    BOOST_FOREACH(ast_light l, value)
    {
        l.apply_visitor(*this);
    }
    out << "}";
}

void AstVisitor::operator()(ast_intersectable value) const
{
    value.apply_visitor(*this);
}

void AstVisitor::operator ()(ast_literal_material l) const
{
    l.apply_visitor(*this);
}

void AstVisitor::operator()(std::string value) const
{
    out << value;
}

void AstVisitor::operator()(ast_intersectable_list value) const
{
    out << "intersectables{";
    BOOST_FOREACH(ast_intersectable i, value.children)
    {
        i.apply_visitor(*this);
    }
    out << "}";
}
