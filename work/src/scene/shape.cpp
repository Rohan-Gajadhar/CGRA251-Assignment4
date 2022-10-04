
// std
#include <algorithm>
#include <utility>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// project
#include "shape.hpp"


using namespace glm;


RayIntersection AABB::intersect(const Ray &ray) {
	RayIntersection intersect;
	vec3 rel_origin = ray.origin - m_center;

	// start magic
	// x
	float rdx_inv = 1 / ray.direction.x;
	float tx1 = (-m_halfsize.x - rel_origin.x) * rdx_inv;
	float tx2 = (m_halfsize.x - rel_origin.x) * rdx_inv;

	float tmin = std::min(tx1, tx2);
	float tmax = std::max(tx1, tx2);

	// y
	float rdy_inv = 1 / ray.direction.y;
	float ty1 = (-m_halfsize.y - rel_origin.y) * rdy_inv;
	float ty2 = (m_halfsize.y - rel_origin.y) * rdy_inv;

	tmin = std::max(tmin, std::min(ty1, ty2));
	tmax = std::min(tmax, std::max(ty1, ty2));

	// z
	float rdz_inv = 1 / ray.direction.z;
	float tz1 = (-m_halfsize.z - rel_origin.z) * rdz_inv;
	float tz2 = (m_halfsize.z - rel_origin.z) * rdz_inv;

	tmin = std::max(tmin, std::min(tz1, tz2));
	tmax = std::min(tmax, std::max(tz1, tz2));

	if (tmax < tmin) return intersect;
	// end magic

	intersect.m_distance = tmin < 0 ? tmax : tmin;
	intersect.m_position = ray.origin + intersect.m_distance * ray.direction;
	intersect.m_valid = tmax >= 0;
	vec3 work_out_a_name_for_it_later = abs((intersect.m_position - m_center) / m_halfsize);
	float max_v = std::max(work_out_a_name_for_it_later[0], std::max(work_out_a_name_for_it_later[1], work_out_a_name_for_it_later[2]));
	intersect.m_normal = normalize(mix(intersect.m_position - m_center, vec3(0), lessThan(work_out_a_name_for_it_later, vec3(max_v))));
	intersect.m_uv_coord = (abs(intersect.m_normal.x) > 0) ?
		vec2(intersect.m_position.y, intersect.m_position.z) :
		vec2(intersect.m_position.x, intersect.m_position.y + intersect.m_position.z);
	intersect.m_shape = this;

	return intersect;
}


RayIntersection Sphere::intersect(const Ray &ray) {
	RayIntersection intersect;
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Implement the intersection method for Sphere that returns
	// a RayIntersection object with valid == false if the 
	// ray doesn't intersect and true otherwise. If true, then
	// remember to fill out each feild in the object correctly:
	// - m_valid : true if object is itersected
	// - m_distance : distance along the ray of the intersection
	// - m_position : position on the surface of the intersection
	// - m_normal : normal on the surface of the intersection
	// - m_shape : set to "this"
	// - m_uv_coord : texture coordinates (challenge only)
	//-------------------------------------------------------------

	// YOUR CODE GOES HERE
	vec3 L = m_center - ray.origin;
    float tca = glm::dot(L, ray.direction);
    float d = glm::dot(L, L) - tca*tca;
    float thc = glm::sqrt(m_radius*m_radius-d);
    float t0 = tca - thc;
    float t1 = tca + thc;

    if((t0>0 && t1>t0) || t0==t1) {
        intersect.m_valid = true;
        intersect.m_distance = t0;
        intersect.m_position = ray.origin + t0 * ray.direction;
        intersect.m_normal = intersect.m_position - m_center;
        intersect.m_shape = this;
    }
    return intersect;
}

RayIntersection Plane::intersect(const Ray &ray) {
    RayIntersection intersect;
    float denom = glm::dot(ray.direction, m_normal);
    if(denom > 0.0001) {
        intersect.m_valid = true;
        intersect.m_distance = glm::dot(m_point - ray.origin, m_normal) / denom;
        intersect.m_position = ray.origin + intersect.m_distance * ray.direction;
        intersect.m_normal = m_normal;
        intersect.m_shape = this;
    }
    return intersect;
}


RayIntersection Disk::intersect(const Ray &ray) {
    /*float denom = glm::dot(m_normal, ray.direction);
    if (denom > 1e-6) {
        vec3 p0l0 = m_center - ray.origin;
        float t = glm::dot(p0l0, m_normal) / denom;
        if (t >= 0) {
            RayIntersection intersect;
            intersect.m_valid = true;
            intersect.m_distance = t;
            intersect.m_position = ray.origin + t * ray.direction;
            intersect.m_normal = m_normal;
            intersect.m_shape = this;
            vec3 p = intersect.m_position - m_center;
            float d = glm::sqrt(p.x*p.x + p.y*p.y);
            if (d <= m_radius) {
                intersect.m_valid = true;
                intersect.m_distance = t;
                intersect.m_position = ray.origin + t * ray.direction;
                intersect.m_normal = m_normal;
                intersect.m_shape = this;
            }
        }

    }*/
    float t = 0;
    Plane m_plane = Plane(m_center, m_normal);
    RayIntersection intersection = m_plane.intersect(ray);
    if (intersection.m_valid) {
        vec3 d = intersection.m_position - m_center;

        if (glm::length(d) > m_radius) {
            intersection.m_valid = false;
        }
    }
    return intersection;
}

RayIntersection Triangle::intersect(const Ray &ray) {
    RayIntersection intersect;
    vec3 e1 = m_corner2 - m_corner1;
    vec3 e2 = m_corner3 - m_corner1;
    vec3 pvec = glm::cross(ray.direction, e2);
    float det = glm::dot(e1, pvec);
    if (det > -1e-6 && det < 1e-6) {
        return intersect;
    }
    float inv_det = 1.0 / det;
    vec3 tvec = ray.origin - m_corner1;
    float u = glm::dot(tvec, pvec) * inv_det;
    if (u < 0 || u > 1) {
        return intersect;
    }
    vec3 qvec = glm::cross(tvec, e1);
    float v = glm::dot(ray.direction, qvec) * inv_det;
    if (v < 0 || u + v > 1) {
        return intersect;
    }
    float t = glm::dot(e2, qvec) * inv_det;
    if (t > 1e-6) {
        intersect.m_valid = true;
        intersect.m_distance = t;
        intersect.m_position = ray.origin + t * ray.direction;
        intersect.m_normal = glm::normalize(glm::cross(e1, e2));
        intersect.m_shape = this;
    }
    return intersect;
}

