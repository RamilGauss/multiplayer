#ifndef _CUBE_H_
#define _CUBE_H_

#include <windows.h>

#include <d3d9.h>

const int NumVerts = 24;
const int NumInds = 36;
const int NumTriangles = 12;

#define FormatVertex (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct CustomVertex
{
    float x, y, z;
    DWORD color;
};

class Cube
{
public:
    Cube();
    Cube(float x, float y, float z);
    ~Cube();

    int GetInformation(CustomVertex **v, unsigned short **i, int &numVerts, int &numTriangles, int &numInds,
        int &sizeVertex, int &sizeIndex);   // Return the format of vertex
    void SetCoord(float x, float y, float z);
    void SetAngle(float x, float y, float z);
    void Init();

    float x;
    float y;
    float z;
    float angleX;
    float angleY;
    float angleZ;
private:
    CustomVertex verts[NumVerts];       
    unsigned short inds[NumInds];
};

#endif