#pragma once
#ifndef camera2_H
#define camera2_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"


enum class Camera_Movement {

	FORWARD,

	BACKWARD,

	LEFT,

	RIGHT

};



// Default camera values

const float YAW = -90.0f;

const float PITCH = 0.0f;

const float SPEED = 2.5f;

const float SENSITIVITY = 0.1f;

const float FOV = 45.0f;



class camera2

{

public:

	// camera Attributes

	glm::vec3 Position;

	glm::vec3 Front;

	glm::vec3 Up;

	glm::vec3 Right;

	glm::vec3 WorldUp;

	glm::quat FrontQuaternion;

	// euler Angles

	float Yaw;

	float Pitch;

	// camera options

	float MovementSpeed;

	float MouseSensitivity;

	float Fov;



	// constructor with vectors

	camera2(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	// constructor with scalar values

	camera2(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	// returns the view matrix calculated using Euler Angles and the LookAt Matrix

	glm::mat4 GetViewMatrix();



	// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)

	void ProcessKeyboard(Camera_Movement direction, float deltaTime);



	// processes input received from a mouse input system. Expects the offset value in both the x and y direction.

	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);



	// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis

	void ProcessMouseScroll(float yoffset);



	void updateCameraVectorsByQuat1(float xoffset, float yoffset);





	void updateCameraVectorsByQuat2(float xoffset, float yoffset);



	glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);

	void ResetYawAndPitch();



private:

	// calculates the front vector from the Camera's (updated) Euler Angles

	void updateCameraVectors();





};

#endif
