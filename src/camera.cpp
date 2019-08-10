#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

float count = 0;

camera::camera() {
	speed = 2.5f;
	sens = 0.1f;
	zoom = 45.0f;

	pos = glm::vec3(0.0f, 0.0f, 2.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	globalUp = up;
	front = glm::vec3(0.0f, 0.0f, 0.0f);
	isRotating=false;
	update();
}

camera::camera(float speed, float sens, float zoom) : speed(speed), sens(sens), zoom(zoom) {
	pos = glm::vec3(0.0f, 0.0f, 2.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	globalUp = up;
	front = glm::vec3(0.0f, 0.0f, 0.0f);
	isRotating=false;
	update();
}

glm::mat4 camera::getViewMatrix() {
	if (!isRotating) {
		return glm::lookAt(pos, pos+front, up);
	}
	else {
		return glm::lookAt(pos, pivot, up);
	}
}

void camera::translate(movement direction, float deltaTime) {
	if (isRotating) {
		update();
		isRotating=false;
	}
	else {isRotating=false;}
	float v = speed * deltaTime;	
	if (direction==FORWARD) {
		pos+=front*v;
	}
	else if (direction==BACK) {
		pos-=front*v;		
	}
	else if (direction==RIGHT) {
		pos+=right*v;
	}
	else if (direction==LEFT) {
		pos-=right*v;
	}
}

void camera::rotate(movement direction) {
	isRotating = true;
	if (direction==CLOCKWISE) {yaw+=1;}
	else if (direction==ANTICLOCKWISE) {yaw-=1;}
	float r = sqrt(pow((pos.x),2)+pow((pos.z), 2));
	float camX = sin(glm::radians(yaw)) * r;
	float camZ = cos(glm::radians(yaw)) * r;

	pos = glm::vec3(camX, pos.y, camZ);
}

void camera::update() {
	yaw=-90.0f;
	pos = glm::vec3(0.0f, 0.0f, 2.0f);
	glm::vec3 f;
	f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	f.y = sin(glm::radians(pitch));
	f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(f);

	right = glm::normalize(glm::cross(front, globalUp));
	up = glm::normalize(glm::cross(right, front));
}

