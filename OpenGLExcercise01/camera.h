#pragma once
#ifndef  CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <GL/glew.h>
#include <GLFW/glfw3.h>


using namespace glm;
using namespace std;

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;

void  mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

void keyboard_callback(GLFWwindow* window);

class Camera
{
public:



	Camera() {}

	Camera(GLFWwindow* window)
	{
		InitCamera(window);
	}

	vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	vec3 cameraFront = vec3(0, 0, -1);
	vec3 cameraUp = vec3(0, 1, 0);

	//vec3 front;

	float pitch;
	float yaw;
	float roll;


	mat4 updateCameraPos()
	{

		return loolAtByCamera(cameraPos, cameraFront, cameraUp);
	}


	void UpdateCameraRot(GLFWwindow* window)
	{
	
	}


private:

	

	mat4 loolAtByCamera(const vec3& cameraPos, const vec3& cameraFront, const vec3& cameraUp)
	{
		return lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	void InitCamera(GLFWwindow* window)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window, mouse_callback);
	}

};


float lastX = 400, lastY = 300;
bool firstFrame = true;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	if (firstFrame)
	{
		firstFrame = false;
		lastX = xposIn;
		lastY = yposIn;
	}
	float xoffset = xposIn - lastX;
	float yoffset = yposIn - lastY;

	lastX = xposIn;
	lastY = yposIn;


}

void keyboard_callback(GLFWwindow* window)
{

}
#endif 
