#include "scenedumper.h"

#include <boost/variant/apply_visitor.hpp>

#include <iostream>

using namespace std;

struct AstVisitor;
class ArgumentListPrinter;

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
        boost::fusion::for_each(value, ArgumentListPrinter(&first, out));
        out << ')';
    }

private:
    std::ostream & out;
};

class ArgumentListPrinter
{
public:
    ArgumentListPrinter(bool *first, std::ostream & out) : first(first), out(out) {}

    template<typename T>
    void operator()(T t) const
    {
        if(*first)
        {
            *first = false;
        }
        else
        {
            out << ',';
        }
        print(t);
    }

    void print(ast_vector2_literal v) const
    {
        out << '[' << v.x << ' ' << v.y << ']';
    }

    void print(ast_vector3_literal v) const
    {
        out << '[' << v.x << ' ' << v.y << ' ' << v.z << ']';
    }

    void print(ast_vector4_literal v) const
    {
        out << '[' << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w << ']';
    }

    void print(float f) const
    {
        out << f;
    }

    void print(std::string s) const
    {
        out << '"' << s << '"';
    }

    //Needed to make it compile
    void print(ast_box b) const
    {
        print(ast_intersectable(b));
    }

    void print(ast_matrix m) const
    {
        boost::apply_visitor(MatrixPrinter(out), m.first);
        for(ast_basic_matrix& mat: m.mult)
        {
            out << '*';
            boost::apply_visitor(MatrixPrinter(out), mat);
        }
    }

    void print(ast_material m) const;
    void print(ast_intersectable) const;
    void print(ast_csg) const;

private:
    bool * const first;
    std::ostream & out;

};

class AstVisitor : public boost::static_visitor<>
{
public:
    AstVisitor(std::ostream & out) : out(out) {}

    void operator()(std::vector<ast_light> value) const;
    void operator()(ast_intersectable value) const;
    void operator()(ast_literal_material l) const;
    void operator()(std::string value) const;
    void operator()(std::vector<char> value) const;
    void operator()(ast_intersectable_list value) const;
    void operator()(ast_csg value) const;

    template<typename T>
    void operator()(T value) const
    {
        out << T::function_name;
        out << "(";
        bool first = true;
        boost::fusion::for_each(value, ArgumentListPrinter(&first, out));
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
    for(ast_assignment& assignment: assignments)
    {
        dump(assignment);
    }
}


void AstVisitor::operator()(std::vector<ast_light> value) const
{
    out << "lights{";
    for(ast_light& l: value)
    {
        l.apply_visitor(*this);
    }
    out << "}";
}

void AstVisitor::operator()(ast_intersectable value) const
{
    boost::apply_visitor(*this, value);
}

void AstVisitor::operator()(ast_csg value) const
{
    boost::apply_visitor(*this, value);
}

void AstVisitor::operator ()(ast_literal_material l) const
{
    l.apply_visitor(*this);
}

void AstVisitor::operator()(std::vector<char> value) const
{
    out << std::string(value.begin(), value.end());
}

void AstVisitor::operator()(std::string value) const
{
    out << value;
}

void AstVisitor::operator()(ast_intersectable_list value) const
{
    out << "intersectables{";
    for(ast_intersectable& i: value.children)
    {
        boost::apply_visitor(*this, i);
    }
    out << "}";
}

void ArgumentListPrinter::print(ast_material m) const
{
    boost::apply_visitor(AstVisitor(out), m);
}

void ArgumentListPrinter::print(ast_intersectable i) const
{
    boost::apply_visitor(AstVisitor(out), i);
}

void ArgumentListPrinter::print(ast_csg i) const
{
    boost::apply_visitor(AstVisitor(out), i);
}
