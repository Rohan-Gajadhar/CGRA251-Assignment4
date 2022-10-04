
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
    //Initialise diffuse and specular to 0 to prevent colour distortion
    vec3 diffuse = vec3(0, 0, 0);
    vec3 specular = vec3(0, 0, 0);
    vec3 irradiance;
    vec3 incident;
    vec3 normal;

    if(rayIntersect.m_valid) {
        for(size_t i = 0; i < m_scene->lights().size(); i++) {
            //Ambient
            diffuse += m_scene->lights()[i]->ambience() * rayIntersect.m_material->diffuse();

            if(!m_scene->lights()[i]->occluded(m_scene, rayIntersect.m_position)) {
                irradiance = m_scene->lights()[i]->irradiance(rayIntersect.m_position);
                incident = m_scene->lights()[i]->incidentDirection(rayIntersect.m_position);
                normal = rayIntersect.m_normal;

                float firstAngle = glm::max(0.f, dot(-incident, normal));

                //Lambertian
                diffuse += irradiance * firstAngle * rayIntersect.m_material->diffuse();

                vec3 reflection = glm::reflect(incident, normal);

                float secondAngle = glm::max(0.f, dot(-ray.direction, reflection));
                float angleSpec = pow(secondAngle, rayIntersect.m_material->shininess());

                //Blinn-Phong
                specular += irradiance * angleSpec * rayIntersect.m_material->specular();
            }
        }
        return diffuse + specular;
    }

	// no intersection - return background color
	return { 0.3f, 0.3f, 0.4f };
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
    //Initialise diffuse and specular to 0 to prevent colour distortion
    vec3 diffuse = vec3(0, 0, 0);
    vec3 specular = vec3(0, 0, 0);
    vec3 reflectedLight = vec3(0, 0, 0);
    vec3 irradiance;
    vec3 normal;
    vec3 incident;

    if(rayIntersect.m_valid) {
        for(size_t i = 0; i < m_scene->lights().size(); i++) {
            //Ambient
            diffuse += m_scene->lights()[i]->ambience() * rayIntersect.m_material->diffuse();

            if(!m_scene->lights()[i]->occluded(m_scene, rayIntersect.m_position)) {
                irradiance = m_scene->lights()[i]->irradiance(rayIntersect.m_position);
                incident = m_scene->lights()[i]->incidentDirection(rayIntersect.m_position);
                normal = rayIntersect.m_normal;

                float firstAngle = glm::max(0.f, dot(-incident, normal));

                //Lambertian
                diffuse += irradiance * firstAngle * rayIntersect.m_material->diffuse();

                vec3 reflection = glm::reflect(incident, normal);

                float secondAngle = glm::max(0.f, dot(-ray.direction, reflection));
                float angleSpec = pow(secondAngle, rayIntersect.m_material->shininess());

                //Blinn-Phong
                specular += irradiance * angleSpec * rayIntersect.m_material->specular();

                if (depth > 1) {
                    vec3 reflectedDirection = normalize(glm::reflect(ray.direction, rayIntersect.m_normal));

                    Ray reflectionRay = Ray(rayIntersect.m_position, reflectedDirection);

                    vec3 rayReflection = sampleRay(reflectionRay, depth - 1) * rayIntersect.m_material->specular();

                    reflectedLight += rayReflection * (1 - (1 / rayIntersect.m_material->shininess()));
                }
            }
        }
        return diffuse + specular + reflectedLight;
    }

    // no intersection - return background color
    return { 0.3f, 0.3f, 0.4f };
}

/*
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
    //Initialise diffuse and specular to 0 to prevent colour distortion
    vec3 diffuse = vec3(0, 0, 0);
    vec3 specular = vec3(0, 0, 0);
    vec3 reflection = vec3(0, 0, 0);
    vec3 irradiance;
    vec3 normal;
    vec3 incident;

    if(rayIntersect.m_valid) {
        for(size_t i = 0; i < m_scene->lights().size(); i++) {
            //Ambient
            diffuse += m_scene->lights()[i]->ambience() * rayIntersect.m_material->diffuse();

            if(!m_scene->lights()[i]->occluded(m_scene, rayIntersect.m_position)) {
                irradiance = m_scene->lights()[i]->irradiance(rayIntersect.m_position);
                incident = m_scene->lights()[i]->incidentDirection(rayIntersect.m_position);
                normal = rayIntersect.m_normal;

                float firstAngle = glm::max(0.f, dot(-incident, normal));

                //Lambertian
                diffuse += irradiance * firstAngle * rayIntersect.m_material->diffuse();

                vec3 spec_reflection = glm::reflect(incident, normal);

                float secondAngle = glm::max(0.f, dot(spec_reflection, -ray.direction));
                float angleSpec = pow(secondAngle, rayIntersect.m_material->shininess());

                //Blinn-Phong
                specular += irradiance * angleSpec * rayIntersect.m_material->specular();

                if (depth > 1) {
                    vec3 reflectedDirection = glm::reflect(ray.direction, -rayIntersect.m_normal);
                    Ray reflectionRay = Ray(rayIntersect.m_position, normalize(reflectedDirection));
                    vec3 rayReflection = sampleRay(reflectionRay, depth - 1);
                    reflection += rayReflection * (1 - (1 / rayIntersect.m_material->shininess()));
                }
            }
        }
        return diffuse + specular + reflection;
    }

    // no intersection - return background color
    return { 0.3f, 0.3f, 0.4f };
}*/



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