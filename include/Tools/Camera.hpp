#pragma once
#include "MaTrix.hpp"
#include "Vertex.hpp"
#include <vector>


struct Plane
{
    Vertex  normal;
    double distance;
};


struct Camera
{
    Vertex position{};
    Mat4x4 orientation = Mat4x4::Identity();
    std::vector<Plane> clipping_planes{};
};