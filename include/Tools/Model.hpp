#pragma once
#include "Triangle.hpp"
#include "Vector.hpp"
#include <vector>

struct Model
{
    std::vector<Vertex>& vertexes;
    std::vector<Triangle>& triangles;
};

struct Instance
{
    Model& model;
    Vec3 position; 
};