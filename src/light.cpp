#include <glm/vec3.hpp>
#include "light.h"
#include "geometry.h"

light::light(glm::vec3 pos, glm::vec3 colour) : pos(pos), colour(colour) {
	shape.GeometryData::loadFromOBJFile("cube.obj");	
}
