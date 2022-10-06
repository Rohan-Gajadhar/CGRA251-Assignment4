
// glm
#include <glm/gtc/constants.hpp>
#include <glm/gtc/random.hpp>

// std
#include <random>

// project
#include "scene.hpp"
#include "shape.hpp"
#include "light.hpp"
#include "material.hpp"
#include "path_tracer.hpp"


using namespace std;
using namespace glm;


vec3 SimplePathTracer::sampleRay(const Ray &ray, int) {
	// intersect ray with the scene
	RayIntersection intersect = m_scene->intersect(ray);

	// if ray hit something
	if (intersect.m_valid) {

		// simple grey shape shading
		float f = abs(dot(-ray.direction, intersect.m_normal));
		vec3 grey(0.5, 0.5, 0.5);
		return mix(grey / 2.0f, grey, f);
	}

	// no intersection - return background color
	return { 0.3f, 0.3f, 0.4f };
}



vec3 CorePathTracer::sampleRay(const Ray &ray, int) {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Implement a PathTracer that calculates the ambient, diffuse
	// and specular, for the given ray in the scene, using the 
	// Phong lighting model. Give special consideration to objects
	// that occluded from direct lighting (shadow rays). You do
	// not need to use the depth argument for this implementation.
	//-------------------------------------------------------------

	// YOUR CODE GOES HERE
	RayIntersection rayIntersect = m_scene->intersect(ray);
    vec3 diffuse = vec3(0, 0, 0);
    vec3 diffusionReflection;
    vec3 reflection;
    vec3 specularReflection;
    vec3 colour = vec3(0, 0, 0);

    if(!rayIntersect.m_valid) {return { 0.3f, 0.3f, 0.4f };}

    for(size_t i = 0; i < m_scene->lights().size(); i++) {
        std::shared_ptr<Light> light = m_scene->lights().at(i);
        diffuse += rayIntersect.m_material->diffuse()* light->ambience();

        bool occluded = light->occluded(m_scene, rayIntersect.m_position);
        float angle = glm::max(0.0f, dot(-light->incidentDirection(rayIntersect.m_position), rayIntersect.m_normal));

        diffusionReflection = light->irradiance(rayIntersect.m_position) * rayIntersect.m_material->diffuse() * angle;
        reflection = glm::reflect(normalize(light->incidentDirection(rayIntersect.m_position)), normalize(rayIntersect.m_normal));

        angle = glm::max(0.0f, dot(reflection, -ray.direction));
        angle = pow(angle, rayIntersect.m_material->shininess());

        specularReflection = light->irradiance(rayIntersect.m_position) * angle * rayIntersect.m_material->specular();

        colour += diffuse + (occluded ? vec3(0, 0, 0) : (diffusionReflection + specularReflection));
    }
	return colour;
}


vec3 CompletionPathTracer::sampleRay(const Ray &ray, int depth) {
    //-------------------------------------------------------------
    // [Assignment 4] :
    // Using the same requirements for the CorePathTracer add in
    // a recursive element to calculate perfect specular reflection.
    // That is compute the reflection ray off your intersection and
    // sample a ray in that direction, using the result to additionally
    // light your object. To make this more realistic you may weight
    // the incoming light by the (1 - (1/shininess)).
    //-------------------------------------------------------------

    // YOUR CODE GOES HERE
    RayIntersection rayIntersect = m_scene->intersect(ray);
    vec3 diffuse = vec3(0, 0, 0);
    vec3 diffusionReflection;
    vec3 reflection;
    vec3 specularReflection;
    vec3 colour = vec3(0, 0, 0);
    vec3 reflectionColour = vec3(0, 0, 0);

    if(!rayIntersect.m_valid) {return { 0.3f, 0.3f, 0.4f };}

    for(size_t i = 0; i < m_scene->lights().size(); i++) {
        std::shared_ptr<Light> light = m_scene->lights().at(i);
        diffuse += rayIntersect.m_material->diffuse() * light->ambience();

        bool occluded = light->occluded(m_scene, rayIntersect.m_position);
        float angle = glm::max(0.0f, dot(-light->incidentDirection(rayIntersect.m_position), rayIntersect.m_normal));

        diffusionReflection = light->irradiance(rayIntersect.m_position) * rayIntersect.m_material->diffuse() * angle;
        reflection = glm::reflect(normalize(light->incidentDirection(rayIntersect.m_position)), normalize(rayIntersect.m_normal));

        angle = glm::max(0.0f, dot(reflection, -ray.direction));
        angle = pow(angle, rayIntersect.m_material->shininess());

        specularReflection = light->irradiance(rayIntersect.m_position) * angle * rayIntersect.m_material->specular();

        colour += diffuse;
        if(!occluded) {colour += diffusionReflection + specularReflection;}
    }
    if(depth > 1) {
        reflectionColour = CompletionPathTracer::sampleRay(Ray(rayIntersect.m_position, normalize(glm::reflect(
                        normalize(ray.direction), normalize(rayIntersect.m_normal)))), depth - 1);
        colour += reflectionColour * rayIntersect.m_material->specular() * (1 - (1 / rayIntersect.m_material->shininess()));
    }
    return colour;
}

vec3 ChallengePathTracer::sampleRay(const Ray &ray, int depth) {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Implement a PathTracer that calculates the diffuse and 
	// specular, for the given ray in the scene, using the 
	// Phong lighting model. Give special consideration to objects
	// that occluded from direct lighting (shadow rays).
	// Implement support for textured materials (using a texture
	// for the diffuse portion of the material).
	//
	// EXTRA FOR EXPERTS :
	// Additionally implement indirect diffuse and specular instead
	// of using the ambient lighting term.
	// The diffuse is sampled from the surface hemisphere and the
	// specular is sampled from a cone of the phong lobe (which
	// gives a glossy look). For best results you need to normalize
	// the lighting (see http://www.thetenthplanet.de/archives/255)
	//-------------------------------------------------------------

	// YOUR CODE GOES HERE
	// ...

	// no intersection - return background color
	return { 0.3f, 0.3f, 0.4f };
}