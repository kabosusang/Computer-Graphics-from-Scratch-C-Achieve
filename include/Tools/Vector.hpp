#pragma once
#include "Color.hpp"
#include <cmath>
#include <cstdint>


struct Vec4 {
	float x;
	float y;
	float z;
	float w;
};

struct Vec3 {
	float x;
	float y;
	float z;
};

struct Vec2 {
	float x;
	float y;
};

//向量点乘
constexpr float VDotProduct(Vec3 v1, Vec3 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
//向量相减
//v1 - v2
constexpr Vec3 VSubtract(Vec3 v1, Vec3 v2) {
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

//向量相加
constexpr Vec3 VAdd(Vec3 v1, Vec3 v2) {
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

//向量长度
constexpr auto VLength(Vec3 vec) {
	return std::sqrt(VDotProduct(vec, vec));
}

//向量乘数值
constexpr Vec3 VMutiply(float k, Vec3 vec) {
	return { k * vec.x, k * vec.y, k * vec.z };
}

constexpr Vec3 VClamp(Vec3 vec) {
	return {
		(vec.x < 0) ? 0 : (vec.x > 255) ? 255
										: vec.x,
		(vec.y < 0) ? 0 : (vec.y > 255) ? 255
										: vec.y,
		(vec.z < 0) ? 0 : (vec.z > 255) ? 255
										: vec.z
	};
}


constexpr Color CClamp(Color color) {
	return {
		(color.r < (uint8_t)0) ? (uint8_t)0 : (color.r > (uint8_t)255) ? (uint8_t)255
										: color.r,
		(color.g < (uint8_t)0) ? (uint8_t)0 : (color.g > (uint8_t)255) ? (uint8_t)255
										: color.g,
		(color.b < (uint8_t)0) ? (uint8_t)0 : (color.r > (uint8_t)255) ? (uint8_t)255
										: color.b,
        255
	};
}



constexpr Vec3 VNormalize(Vec3 vec) {
	return VMutiply(1.0 / VLength(vec), vec);
}

//向量转换颜色
constexpr Color Vec3ToColor(Vec3 v) {
	return Color{
		static_cast<uint8_t>(v.x),
		static_cast<uint8_t>(v.y),
		static_cast<uint8_t>(v.z),
		255
	};
}

constexpr Vec3 ColorToVec3(Color c) {
    return Vec3{
        static_cast<float>(c.r),
        static_cast<float>(c.g),
        static_cast<float>(c.b)
    };
}
