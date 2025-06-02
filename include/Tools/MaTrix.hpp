#pragma once
#include <cmath>
#include "Vector.hpp"
#include "Vertex.hpp"

constexpr double PI = 3.1415926;

struct Mat4x4 {
	double data[4][4];

	// 静态方法创建单位矩阵
	static Mat4x4 Identity() {
		return Mat4x4({ { { 1, 0, 0, 0 },
				{ 0, 1, 0, 0 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 0, 1 } } });
	}
};

//矩阵相乘
inline Mat4x4 operator*(const Mat4x4& rhs, const Mat4x4& ihs) {
	Mat4x4 result{};

	for (int i = 0; i < 4; ++i) { // 遍历结果矩阵的行
		for (int j = 0; j < 4; ++j) { // 遍历结果矩阵的列
			for (int k = 0; k < 4; ++k) { // 内积求和
				result.data[i][j] += rhs.data[i][k] * ihs.data[k][j];
			}
		}
	}
	return result;
}

inline Vec4 operator*(const Mat4x4& mat,const Vec4& vec4) {
	return {
		mat.data[0][0] * vec4.x + mat.data[0][1] * vec4.y + mat.data[0][2] * vec4.z + mat.data[0][3] * vec4.w,
		mat.data[1][0] * vec4.x + mat.data[1][1] * vec4.y + mat.data[1][2] * vec4.z + mat.data[1][3] * vec4.w,
		mat.data[2][0] * vec4.x + mat.data[2][1] * vec4.y + mat.data[2][2] * vec4.z + mat.data[2][3] * vec4.w,
		mat.data[3][0] * vec4.x + mat.data[3][1] * vec4.y + mat.data[3][2] * vec4.z + mat.data[3][3] * vec4.w
	};
}

inline Vertex4 operator*(const Mat4x4& mat,const Vertex4& vec4) {
	return {
		mat.data[0][0] * vec4.x + mat.data[0][1] * vec4.y + mat.data[0][2] * vec4.z + mat.data[0][3] * vec4.w,
		mat.data[1][0] * vec4.x + mat.data[1][1] * vec4.y + mat.data[1][2] * vec4.z + mat.data[1][3] * vec4.w,
		mat.data[2][0] * vec4.x + mat.data[2][1] * vec4.y + mat.data[2][2] * vec4.z + mat.data[2][3] * vec4.w,
		mat.data[3][0] * vec4.x + mat.data[3][1] * vec4.y + mat.data[3][2] * vec4.z + mat.data[3][3] * vec4.w
	};
}



//绕着Y轴旋转
constexpr Mat4x4 MakeOYRotationMatrix(double degress) {
	const auto cos = std::cos(degress * PI / 180.0f);
	const auto sin = std::sin(degress * PI / 180.0f);

	return { { { cos, 0.0, -sin, 0.0 },
			{ 0.0, 1.0, 0.0, 0.0 },
			{ sin, 0.0, cos, 0.0 },
			{ 0.0, 0.0, 0.0, 1.0 } } };
}

//缩放矩阵
constexpr Mat4x4 MakeScalingMatrix(double scale) {
	return { { { scale, 0.0, 0.0, 0.0 },
			{ 0.0, scale, 0.0, 0.0 },
			{ 0.0, 0.0, scale, 0.0 },
			{ 0.0, 0.0, 0.0, 1.0 } } };
}

//平移矩阵
constexpr Mat4x4 MakeTranslationMatrix(Vec3 translation) {
	return { { { 1, 0.0, 0.0, translation.x },
			{ 0.0, 1, 0.0, translation.y },
			{ 0.0, 0.0, 1, translation.z },
			{ 0.0, 0.0, 0.0, 1.0 } } };
}

//矩阵转置
constexpr Mat4x4 Transposed(const Mat4x4& mat4) {
	Mat4x4 result{};
	for (auto i = 0; i < 4; i++) {
		for (auto j = 0; j < 4; j++) {
			result.data[i][j] = mat4.data[j][i];
		}
	}
	return result;
}
