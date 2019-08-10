#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include "light.h"
#include "geometry.h"

light::light(glm::vec3 pos, glm::vec3 colour) : pos(pos), colour(colour) {
	shape.GeometryData::loadFromOBJFile("cube.obj");	
}

void light::orbit(int direction, float speed) {
	if (direction == -1) {angle-=speed;}	
	else {angle+=speed;}

	float r = sqrt(pow(pos.y, 2)+pow(pos.z,2));
	float camY = sin(glm::radians(angle)) * r;
	float camZ = cos(glm::radians(angle)) * r;
	
	pos = glm::vec3(pos.x, camY, camZ);
}
