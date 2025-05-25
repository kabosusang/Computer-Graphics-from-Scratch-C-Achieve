#pragma once
#include <utility>
#include <vector>
#include <Tools/Shape.hpp>
#include <Tools/Light.hpp>

class RayTracingScene {
public:
	explicit RayTracingScene(std::vector<Sphere> sphere = {}) {
		this->spheres_ = std::move(sphere);
	}

	explicit RayTracingScene(std::vector<Sphere>&& spheres) :
			spheres_(std::move(spheres)) {} // 移动构造

//Spheres
	const std::vector<Sphere>& GetSpheres() const { return spheres_; }
	void AddSphere(Sphere sphere = {}) {
		spheres_.emplace_back(std::move(sphere));
	}

//Lights
    const std::vector<Light>& GetLights() const { return lights_; }
	void AddLight(Light light = {}) {
		lights_.emplace_back(std::move(light));
	}

	RayTracingScene(const RayTracingScene&) = delete;
	RayTracingScene& operator=(const RayTracingScene&) = delete;

public:
	std::vector<Sphere> spheres_;
    std::vector<Light> lights_;
};