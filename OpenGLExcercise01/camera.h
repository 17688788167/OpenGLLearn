#pragma once
#ifndef  CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;

using namespace glm;
using namespace std;



class Camera
{
public:

	string name = "camera";
	//四元数表示旋转
	float x, y, z, w;
	quat MyQuaternion;
	vec3 EulerAngles = vec3(90.0f, 45.0f, 0.0f);





	//视图矩阵相关属性
	vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f);
	vec3 Front = vec3(0, 0, -1);
	vec3 Up = vec3(0, 1, 0);

	vec3 Right;
	vec3 WorldUp=vec3(0,1,0);

	float MovementSpeed=2.5f;
	float MouseSensitivity=0.05f;

	float Pitch = 0.0f;
	float Yaw = -90.0f;
	float Roll = 0.0f;

	mat4 view;

	//透视矩阵相关属性
	float fov = 45.0f;
	float aspect = 800.0f / 600.0f;
	float znear = 0.1f;
	float zfar = 100.0f;
	mat4 projection;

	Camera(float x)
	{
		updateCameraVectors();
	}

	Camera()
	{
		MyQuaternion = quat(x, y, z, w);
		MyQuaternion = quat(EulerAngles);
		updateCameraVectors();
	}

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}


	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;

		updateCameraVectors();
	}

	mat4 updateCameraPos(vec3 PosOffset, float deltaTimes)
	{
		float speed = MovementSpeed * deltaTimes;
		//vec3 a = PosOffset.x * Right * speed;
		//vec3 b= PosOffset.z * Front * speed;
		Position += PosOffset.x * Right * speed;
		Position += PosOffset.y * Up * speed;
		Position += PosOffset.z * Front * speed;

	//	cout << Right.x << <<endl;
		return GetViewMatrix();
	}




	mat4 UpdateCameraRot(float xOffset, float yOffset)
	{

		xOffset *= MouseSensitivity;
		yOffset *= MouseSensitivity;

		Yaw += xOffset;
		Pitch = clamp(Pitch - yOffset, -89.0f, 89.0f);

		updateCameraVectors();

		return GetViewMatrix();
	}


	mat4 UpdateCameraFov(float yoffset)
	{
		fov = clamp(fov -= yoffset, 1.0f, 90.0f);
		return GetProjectionMatrix();
	}

private:



	mat4 GetViewMatrix()
	{
		view = glm::lookAt(Position, Position + Front, Up);
		return view;
	}

	mat4 GetProjectionMatrix()
	{
		projection = perspective(radians(fov), aspect, znear, zfar);
		return projection;
	}
	void updateCameraVectors()
	{
		vec3 front;
		front.x = cos(radians(Pitch)) * cos(radians(Yaw));
		front.y = sin(radians(Pitch));
		front.z = cos(radians(Pitch)) * sin(radians(Yaw));
		Front = normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));

	
		//cout << "Postion:" << Position.x << Position.y << Position.z << endl;
		//cout << "Front:" << Front.x << Front.y << Front.z << endl;
		//cout << "Up:" << Up.x << Up.y << Up.z << endl;
	}
};




#endif 
