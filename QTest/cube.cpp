#include "cube.h"

Cube::Cube()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    angleX = 0.0f;
    angleY = 0.0f;
    angleZ = 0.0f;
    Init();
}

Cube::Cube(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    angleX = 0.0f;
    angleY = 0.0f;
    angleZ = 0.0f;
    Init();
}

Cube::~Cube()
{
}

void Cube::Init()
{
    CustomVertex tempVerts[] = {
        -1.0f, -1.0f, -1.0f, 0xffffffff,
        -1.0f,  1.0f, -1.0f, 0xffffffff,
        1.0f,  1.0f, -1.0f, 0xffffffff,
        1.0f, -1.0f, -1.0f, 0xffffffff,

        1.0f, -1.0f, -1.0f, 0xff00ffff,
        1.0f,  1.0f, -1.0f, 0xff00ffff,
        1.0f,  1.0f,  1.0f, 0xff00ffff,
        1.0f, -1.0f,  1.0f, 0xff00ffff,

        1.0f, -1.0f,  1.0f, 0xffffff00,
        1.0f,  1.0f,  1.0f, 0xffffff00,
        -1.0f,  1.0f,  1.0f, 0xffffff00,
        -1.0f, -1.0f,  1.0f, 0xffffff00,

        -1.0f, -1.0f,  1.0f, 0xffff0000,
        -1.0f,  1.0f,  1.0f, 0xffff0000,
        -1.0f,  1.0f, -1.0f, 0xffff0000,
        -1.0f, -1.0f, -1.0f, 0xffff0000,

        -1.0f,  1.0f, -1.0f, 0xff00ff00,
        -1.0f,  1.0f,  1.0f, 0xff00ff00,
        1.0f,  1.0f,  1.0f, 0xff00ff00,
        1.0f,  1.0f, -1.0f, 0xff00ff00,

        1.0f, -1.0f, -1.0f, 0xff0000ff,
        1.0f, -1.0f,  1.0f, 0xff0000ff,
        -1.0f, -1.0f,  1.0f, 0xff0000ff,
        -1.0f, -1.0f, -1.0f, 0xff0000ff,
    };
    memcpy(verts, tempVerts, sizeof(CustomVertex) * NumVerts);

    unsigned short tempInds[]={
        0,1,3,3,1,2,
        4,5,7,7,5,6,
        8,9,11,11,9,10,
        12,13,15,15,13,14,
        16,17,19,19,17,18,
        20,21,23,23,21,22,
    };
    memcpy(inds, tempInds, sizeof(unsigned short) * NumInds);
}

void Cube::SetCoord(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Cube::SetAngle(float x, float y, float z)
{
    angleX = x;
    angleY = y;
    angleZ = z;
}

int Cube::GetInformation(CustomVertex **v, unsigned short **i, int &numVerts, int &numTriangles, int &numInds,
                          int &sizeVertex, int &sizeIndex)
{
    *v = (CustomVertex*)verts;
    *i = (unsigned short*)inds;
    numVerts = NumVerts;
    numTriangles = NumTriangles;
    numInds = NumInds;
    sizeVertex = sizeof(CustomVertex);
    sizeIndex = sizeof(unsigned short);
    return FormatVertex;
}