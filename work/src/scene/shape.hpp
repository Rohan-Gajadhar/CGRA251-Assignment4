
#pragma once

// glm
#include <glm/glm.hpp>

// project
#include "ray.hpp"
#include "scene.hpp"


class Shape {
public:
	virtual RayIntersection intersect(const Ray &ray) = 0;
};


class AABB : public Shape {
private:
	glm::vec3 m_center;
	glm::vec3 m_halfsize;

public:
	AABB(const glm::vec3 &c, float hs) : m_center(c), m_halfsize(hs) { }
	AABB(const glm::vec3 &c, const glm::vec3 &hs) : m_center(c), m_halfsize(hs) { }
	virtual RayIntersection intersect(const Ray &ray) override;
};


class Sphere : public Shape {
private:
	glm::vec3 m_center;
	float m_radius;

public:
	Sphere(const glm::vec3 &c, float radius) : m_center(c), m_radius(radius) { }
	virtual RayIntersection intersect(const Ray &ray) override;
};

//-------------------------------------------------------------
// [Assignment 4] :
// Implement the following additional Shapes :
// - Plane
// - Disk
// - Triangle
// Follow the pattern shown by AABB and Sphere for implementing
// a class that subclasses Shape making sure that you implement
// the intersect method for each new Shape.
//-------------------------------------------------------------

// YOUR CODE GOES HERE
class Plane : public Shape {
private:
    glm::vec3 m_normal;
    glm::vec3 m_point;

public:
    Plane(const glm::vec3 &n, const glm::vec3 &p) : m_normal(n), m_point(p) { }
    virtual RayIntersection intersect(const Ray &ray) override;
};

class Disk : public Shape {
private:
    glm::vec3 m_center;
    glm::vec3 m_normal;
    float m_radius;

public:
    Disk(const glm::vec3 &c, const glm::vec3 &n, float r) : m_center(c), m_normal(n), m_radius(r) { }
    virtual RayIntersection intersect(const Ray &ray) override;
};

class Triangle : public Shape {
private:
    glm::vec3 m_corner1;
    glm::vec3 m_corner2;
    glm::vec3 m_corner3;

public:
    Triangle(const glm::vec3 &c1, const glm::vec3 &c2, const glm::vec3 &c3) : m_corner1(c1), m_corner2(c2), m_corner3(c3) { }
    virtual RayIntersection intersect(const Ray &ray) override;
};


