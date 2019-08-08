#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

camera::camera() {
	speed = 2.5f;
	sens = 0.1f;
	zoom = 45.0f;

	pos = glm::vec3(0.0f, 0.0f, 2.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	globalUp = up;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	update();
}

camera::camera(float speed, float sens, float zoom) : speed(speed), sens(sens), zoom(zoom) {
	pos = glm::vec3(0.0f, 0.0f, 2.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	globalUp = up;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	update();
}

glm::mat4 camera::getViewMatrix() {return glm::lookAt(pos, pos+front, up);}

void camera::translate(glm::vec3 direction, float deltaTime) {
	float v = speed * deltaTime;	
	pos += direction * v;
	update();
}

void camera::update() {
	glm::vec3 f;
	f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	f.y = sin(glm::radians(pitch));
	f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(f);

	right = glm::normalize(glm::cross(front, globalUp));
	up = glm::normalize(glm::cross(right, front));
}

