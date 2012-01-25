#ifndef __ObjReader_h__
#define __ObjReader_h__

#include <QtCore/QSharedPointer>

#include "global.h"

class ObjReader
{
  public:
    static Intersectable* getMesh(const char* fileName, QSharedPointer< Material > material);
    
  private:
    static void readObj(const char* fileName, int &nVertices, float **vertices, float **normals, float **texcoords, int &nIndices, int **indices);
    static void get_indices(char *word, int *vindex, int *tindex, int *nindex);
};

#endif

