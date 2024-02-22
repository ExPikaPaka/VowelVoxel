#pragma once
#include "..\dataTypes.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ent {
	namespace render {
		// Defines several possible options for camera movement
		enum class Camera_Movement {
			FORWARD,
			BACKWARD,
			LEFT,
			RIGHT,
			UP,
			DOWN
		};

		// Defines several possible options for camera movement alignment
		enum class Camera_Alignment {
			AXIS,
			FREECAM
		};

		// Default camera values
		const f32 YAW = 0.0f;;
		const f32 PITCH = 0.0f;
		const f32 SPEED = 3.6f; // Usual human running speed
		const f32 SENSITIVITY = 0.1f; // Mouse sensitivity
		const f32 FOV = 80.0f;

		// An abstract Camera class
		class Camera {
		public:
			// Camera Attributes
			f32v3 position;
			f32v3 front, up, right;
			f32v3 worldUp;
			// Euler Angles
			f32 yaw, pitch;
			// Camera options
			f32 speed;
			f32 mouseSensitivity;
			f32 fov;

			Camera_Alignment alignment;

			// Customizable constructor
			Camera(f32v3 position = f32v3(0.0f), f32v3 front = f32v3(0.0f, 0.0f, -1.0f), f32v3 worldUp = f32v3(0.0f, 0.0f, 1.0f),
				f32 speed = SPEED, f32 fov = FOV, f32 mouseSensitivity = SENSITIVITY, f32 yaw = YAW, f32 pitch = PITCH);

			// Returns view matrix
			glm::mat4 getViewMatrix();
			// Updates camera position
			void processMovement(Camera_Movement direction, f32 deltaTime);
			// Updates Euler angles of camera
			void processMouseMovement(f32 xoffset, f32 yoffset, bool constrainPitch = true);
			// Updates FOV
			void processMouseScroll(f32 yoffset);
			// Updates camera vectors depending on values
			void updateCameraVectors();
			// Makes camera look at specified point
			void lookAt(f32v3 target);
		};

	}
}

