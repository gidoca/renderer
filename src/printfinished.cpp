#include "printfinished.h"

#include <iostream>

using namespace std;

void PrintFinished::printTime()
{
    cerr << "Rendering complete, " << timeElapsed.elapsed() / 1000 << "s elapsed." << endl;
}
