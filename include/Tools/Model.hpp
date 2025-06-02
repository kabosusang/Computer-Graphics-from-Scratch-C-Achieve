#pragma once
#include "MaTrix.hpp"
#include "Triangle.hpp"
#include "Vector.hpp"
#include "Vertex.hpp"
#include <vector>


struct Model {
	std::vector<Vertex4> vertexes;
	std::vector<Triangle> triangles;
    Vertex4 bounds_center{};
    float bounds_radius{};
};

struct Instance {
	Model& model;
	Vec3 position;
	Mat4x4 orientation;
	double scale = 1.0f;

	Mat4x4 getTransform() {
		//平移矩阵
        return MakeTranslationMatrix(this->position) *
		(this->orientation* MakeScalingMatrix(this->scale));
    }
};