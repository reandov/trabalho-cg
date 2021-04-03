#pragma once
#include <cmath>
#include <iostream>

// Header for Vector
struct vec3
{
	float x, y, z;
	vec3();
	vec3(float x_, float y_, float z_);
	vec3 operator+(const vec3 &v) const;
	vec3 operator-(const vec3 &v) const;
	vec3 operator*(float k) const;
	float mag();
	void normalize();
	vec3 vectorialProduct(vec3 v);
	void print();
};

struct vec2
{
	float x, y;
	vec2() : x(0.0), y(0.0) {}
	vec2(float x_, float y_) : x(x_), y(y_) {}
};