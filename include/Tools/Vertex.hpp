#pragma once
#include "Vector.hpp"

struct Vertex;

struct Vertex4 {
	double x;
	double y;
	double z;
    double w;
    // 显式转换为 Vec4
    explicit operator Vec4() const {
        return Vec4{x, y, z,w};
    }

    // 显式转换为 Vec3
    explicit operator Vec3() const {
        return Vec3{static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)};
    }
};


struct Vertex {
	float x;
	float y;
	float z;

    // 显式转换为 Vec3
    explicit operator Vec3() const {
        return Vec3{x, y, z};
    }

    // 显式转换为 Vec4
    explicit operator Vec4() const {
        return Vec4{x, y, z,1};
    }

    explicit operator Vertex4() const {
        return Vertex4{x, y, z,1};
    }
};






