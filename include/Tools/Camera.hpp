#pragma once

#include "MaTrix.hpp"
#include "Vertex.hpp"

struct Camera
{
    Vertex position{};
    Mat4x4 orientation = Mat4x4::Identity();
};