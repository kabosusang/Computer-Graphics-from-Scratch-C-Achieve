#pragma once
#include <Tools/Shape.hpp>
#include <utility>
#include <vector>

class RayTracingScene {
public:
	explicit RayTracingScene(std::vector<Sphere> sphere = {}) {
		this->spheres_ = std::move(sphere);
	}

	explicit RayTracingScene(std::vector<Sphere>&& spheres) :
			spheres_(std::move(spheres)) {} // 移动构造

	const std::vector<Sphere>& GetSpheres() const { return spheres_; }

	void AddSphere(Sphere sphere = {}) {
		spheres_.emplace_back(std::move(sphere));
	}

	RayTracingScene(const RayTracingScene&) = delete;
	RayTracingScene& operator=(const RayTracingScene&) = delete;

public:
	std::vector<Sphere> spheres_;
};