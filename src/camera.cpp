#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

camera::camera() = default;
camera::camera(glm::vec3 &pos, glm::quat &orientation) : pos(pos), orientation(orientation) {}

glm::vec3 camera::getPos() {return pos;}
glm::quat camera::getOrientation() {return orientation;}

glm::mat4 camera::getViewMatrix() {return glm::translate(glm::mat4_cast(orientation), pos);}

void camera::translate(float x, float y, float z) {pos += glm::vec3(x, y, z) * orientation;}
void camera::rotate(float angle, float x, float y, float z) {orientation *= glm::angleAxis(angle, glm::vec3(x, y, z) * orientation);}

void camera::yaw(float angle) { rotate(angle, 0.0f, 1.0f, 0.0f);}
void camera::pitch(float angle) { rotate(angle, 1.0f, 0.0f, 0.0f);}
void camera::roll(float angle) { rotate(angle, 0.0f, 0.0f, 1.0f);}
