#ifndef CAMERA_H
#define CAMERA_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

class camera {
	private:
		glm::vec3 pos;
		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 globalUp;

		float yaw = -90.0f;
		float pitch = 0.0f;

		float speed;
		float sens;
		float zoom;

	public:
		camera();
		camera(float speed, float sens, float zoom);

		glm::mat4 getViewMatrix();

		void update();
		void translate(glm::vec3 direction, float deltaTime);
		void rotate();
};

#endif
