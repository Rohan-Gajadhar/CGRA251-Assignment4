
#include <iostream>

// glm
#include <glm/gtc/matrix_transform.hpp>

// project
#include "camera.hpp"
#include "opengl.hpp"


using namespace std;
using namespace glm;


void Camera::setPositionOrientation(const vec3 &pos, float yaw, float pitch) {
	m_position = pos;
	m_yaw = yaw;
	m_pitch = pitch;

	// update rotation matrix (based on yaw and pitch)
	m_rotation = rotate(mat4(1), m_yaw, vec3(0, 1, 0)) * rotate(mat4(1), m_pitch, vec3(1, 0, 0));
}


Ray Camera::generateRay(const vec2 &pixel) {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Generate a ray in the scene using the camera position,
	// rotation field of view on the y axis (fovy) and the image
	// size. The pixel is given in image coordinates [0, imagesize]
	// This COULD be done by first creating the ray in ViewSpace
	// then transforming it by the position and rotation to get
	// it into worldspace.
	//-------------------------------------------------------------
	
	Ray ray;

	// YOUR CODE GOES HERE

    //set ray position to camera position
    ray.origin = m_position;

    float pixel_ndc_x = (pixel.x + 0.5) / m_image_size.x;
    float pixel_ndc_y = (pixel.y + 0.5) / m_image_size.y;

    float pixel_screen_x = 2 * pixel_ndc_x - 1;
    float pixel_screen_y = 2 * pixel_ndc_y - 1;

    float image_aspect_ratio = m_image_size.x / m_image_size.y;

    float pixel_camera_x = pixel_screen_x * image_aspect_ratio * tan(m_fovy/2);
    float pixel_camera_y = (pixel_screen_y) * tan(m_fovy/2);

    //z is unit distance away in negative direction
    float z = -1;

    //w is 0 as this is a direction
    float w = 0;

    //get unit vector of direction relative to camera
    vec4 camera_direction = glm::normalize(glm::vec4(pixel_camera_x, pixel_camera_y, z, w));

    //multiply camera direction by camera rotation to get ray direction in world
    vec4 world_direction = camera_direction * m_rotation;

    ray.direction = vec3(world_direction.x, world_direction.y, world_direction.z);

    return ray;
}
