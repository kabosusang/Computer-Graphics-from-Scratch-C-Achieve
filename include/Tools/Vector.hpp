#pragma once

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
constexpr float DotProduct(Vec3 v1,Vec3 v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
//向量相减
constexpr Vec3 Subtract(Vec3 v1,Vec3 v2)
{
    return {v1.x - v2.x,v1.y - v2.y,v1.z - v2.z};
}



