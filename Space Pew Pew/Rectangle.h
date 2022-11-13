#pragma once
#include "Mesh.h"
class Rectangle :
    public Mesh
{
private:
    float m_Vertices[20];
    unsigned int m_Indices[6];

public:
    Rectangle();

};

