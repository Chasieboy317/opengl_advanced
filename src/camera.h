#ifndef CAMERA_H
#define CAMERA_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

class camera {
	private:
		glm::vec3 pos;
		glm::quat orientation;
	public:
		camera();
		camera(glm::vec3 &pos, glm::quat &orientation);

		glm::vec3 getPos();
		glm::quat getOrientation();

		glm::mat4 getViewMatrix();

		void translate(float x, float y, float z);
		void rotate(float angle, float x, float y, float z);
		void yaw(float angle);
		void pitch(float angle);
		void roll(float angle);
};

#endif
