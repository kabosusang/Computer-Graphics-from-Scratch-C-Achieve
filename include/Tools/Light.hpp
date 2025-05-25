#pragma once
#include "Vector.hpp"
#include <optional>

enum class LightType {
	ambient = 0,
	directional = 1,
	point = 2,
};

struct Light {
	LightType type;
	float intensity;
    std::optional<Vec3> position;
    std::optional<Vec3> direction;
};