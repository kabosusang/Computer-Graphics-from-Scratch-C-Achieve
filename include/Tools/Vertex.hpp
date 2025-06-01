#pragma once
#include "Vector.hpp"


struct Vertex {
	float x;
	float y;
	float z;

    // 显式转换为 Vec3
    explicit operator Vec3() const {
        return Vec3{x, y, z};
    }
};


struct Vertex4 {
	float x;
	float y;
	float z;
    float w;
    // 显式转换为 Vec3
    explicit operator Vec4() const {
        return Vec4{x, y, z,w};
    }
};


