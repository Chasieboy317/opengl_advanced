#ifndef CAMERA_H
#define CAMERA_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

enum movement {
	FORWARD,
	BACK,
	RIGHT,
	LEFT,
	CLOCKWISE,
	ANTICLOCKWISE
};

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

		bool isRotating=false;

	public:
		camera();
		camera(float speed, float sens, float zoom);

		glm::mat4 getViewMatrix();

		void update();
		void translate(movement direction, float deltaTime);
		void rotate(movement direction);
};

#endif
