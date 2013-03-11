#include "sceneparser.h"
#include "bvh.h"
#include "scenedumper.h"

#include <iostream>
#include <boost/foreach.hpp>

using namespace std;

int main(int argc, char** argv)
{
    if(argc == 0)
    {
        cerr << "Usage: dumpbvh <scene>" << endl;
    }

    SceneGrammar parser;
    if(!parser.parse(argv[1]))
    {
      cerr << "Failed to parse scene\n";
      return -1;
    }
    std::vector<ast_assignment> scene = parser.getAst();
    resolveVars(scene);

    SceneDumper d;
    d.dump(scene);
}
