#include "Camera.h"

ent::render::Camera::Camera(f32v3 position, f32v3 front, f32v3 worldUp, f32 speed, f32 fov, f32 mouseSensitivity, f32 yaw, f32 pitch) {
	this->position = position;
	this->front = front;
	this->speed = speed;
	this->fov = fov;
	this->mouseSensitivity = mouseSensitivity;
	this->yaw = yaw;
	this->pitch = pitch;
	this->worldUp = worldUp;

	updateCameraVectors();
}

glm::mat4 ent::render::Camera::getViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

void ent::render::Camera::processMovement(Camera_Movement direction, f32 deltaTime) {
	f32 velocity = speed * deltaTime;
	f32v3 forward = glm::normalize(f32v3(front.x, front.y, 0));

	if (direction == FORWARD)
		position += forward * velocity;
	if (direction == BACKWARD)
		position -= forward * velocity;
	if (direction == RIGHT)
		position += right * velocity;
	if (direction == LEFT)
		position -= right * velocity;
	if (direction == UP)
		position += worldUp * velocity;
	if (direction == DOWN)
		position -= worldUp * velocity;
}

void ent::render::Camera::processMouseMovement(f32 xoffset, f32 yoffset, bool constrainPitch) {
	yaw += xoffset * mouseSensitivity;
	pitch -= yoffset * mouseSensitivity;

	if (constrainPitch) {
		if (pitch > 89.99) pitch = 89.99f;
		if (pitch < -89.99f) pitch = -89.99f;
	}

	// update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

void ent::render::Camera::processMouseScroll(f32 yoffset) {
	fov -= yoffset;
	if (fov < 1.0f) fov = 1.0f;
	if (fov > 130.0f) fov = 130.0f;

	// update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

void ent::render::Camera::updateCameraVectors() {
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.z = sin(glm::radians(pitch));
	front.y = -sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void ent::render::Camera::lookAt(f32v3 target) {
	// Calculate the new front vector to look at the target
	front = glm::normalize(target - position);

	// Recalculate the pitch and yaw angles from the new front vector
	pitch = glm::degrees(asin(front.z));
	yaw = -glm::degrees(atan2(front.y, front.x));

	// Update camera vectors
	updateCameraVectors();
}
