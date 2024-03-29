#ifndef LIGHT_H
#define LIGHT_H

#include <iostream>
#include <glm/vec3.hpp>
#include "geometry.h"

class light {
	private:
		float angle = 0.0f;
	public:
		glm::vec3 pos;
		glm::vec3 colour;
		GeometryData shape;

		light(glm::vec3 pos, glm::vec3 colour);

		void orbit(int direction, float speed);
};
#endif
