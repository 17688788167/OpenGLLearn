#include "camera2.h"

camera2::camera2(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV)

{

	Position = position;

	WorldUp = up;

	Yaw = yaw;

	Pitch = pitch;

	updateCameraVectors();

}



camera2::camera2(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV)

{

	Position = glm::vec3(posX, posY, posZ);

	WorldUp = glm::vec3(upX, upY, upZ);

	Yaw = yaw;

	Pitch = pitch;

	updateCameraVectors();

}



glm::mat4 camera2::GetViewMatrix()

{

	return glm::lookAt(Position, Position + Front, Up);



}



void camera2::ProcessKeyboard(Camera_Movement direction, float deltaTime)

{

	float velocity = MovementSpeed * deltaTime;

	if (direction == Camera_Movement::FORWARD)

		Position += Front * velocity;

	if (direction == Camera_Movement::BACKWARD)

		Position -= Front * velocity;

	if (direction == Camera_Movement::LEFT)

		Position -= Right * velocity;

	if (direction == Camera_Movement::RIGHT)

		Position += Right * velocity;

}



void camera2::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)

{

	xoffset *= MouseSensitivity;

	yoffset *= MouseSensitivity;



	Yaw += xoffset;

	Pitch += yoffset;



	// make sure that when pitch is out of bounds, screen doesn't get flipped

	if (constrainPitch)

	{

		if (Pitch > 89.0f)

			Pitch = 89.0f;

		if (Pitch < -89.0f)

			Pitch = -89.0f;

	}



	// update Front, Right and Up Vectors using the updated Euler angles

	updateCameraVectors();

}



void camera2::ProcessMouseScroll(float yoffset)

{

	Fov -= (float)yoffset;

	if (Fov < 1.0f)

		Fov = 1.0f;

	if (Fov > 45.0f)

		Fov = 45.0f;

}



/// <summary>

/// upadate CameraVector by Euler

/// </summary>

void camera2::updateCameraVectors()

{

	// calculate the new Front vector

	glm::vec3 front;

	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));

	front.y = sin(glm::radians(Pitch));

	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

	/*

	front.x = cos(glm::radians(Yaw));

	front.y = sin(glm::radians(Pitch));

	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

	*/

	Front = glm::normalize(front);

	// also re-calculate the Right and Up vector

	Right = glm::normalize(glm::cross(Front, WorldUp)); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.

	Up = glm::normalize(glm::cross(Right, Front));

	FrontQuaternion = glm::quat(Front);

}



/// <summary>

/// update CameraVector by Quat(Maybe have some question)

/// </summary>

/// <param name="xoffset"></param>

/// <param name="yoffset"></param>

/// <param name="constrainPitch"></param>

void camera2::updateCameraVectorsByQuat1(float xoffset, float yoffset)

{

	Yaw += 90;

	xoffset *= 0.008f * MouseSensitivity;

	yoffset *= 0.008f * MouseSensitivity;



	Yaw = xoffset;

	Pitch = yoffset;



	// make sure that when pitch is out of bounds, screen doesn't get flipped



	if (Pitch > 90.0f)

		Pitch -= 90.0f;

	if (Pitch < -90.0f)

		Pitch += 90.0f;



	if (Yaw > 90.0f)

		Yaw -= 90.0f;

	if (Yaw < -90.0f)

		Yaw += 90.0f;





	glm::vec3 axis = glm::cross(Front, Up);

	glm::quat pitchQuat = glm::angleAxis(Pitch, axis);

	//determine heading quaternion from the camera up vector and the heading angle

	axis = glm::cross(Front, axis);

	glm::quat yawQuat = glm::angleAxis(Yaw, Up);

	//add the two quaternions

	glm::quat combinedRotation = pitchQuat * yawQuat;



	Front = glm::rotate(combinedRotation, Front);





	Front = glm::normalize(Front);

	Right = glm::normalize(glm::cross(Front, WorldUp)); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.

	Up = glm::normalize(glm::cross(Right, Front));



	Yaw -= 90;

}



/// <summary>

/// update CameraVector by Quat (right)

/// </summary>

/// <param name="xoffset"></param>

/// <param name="yoffset"></param>

void camera2::updateCameraVectorsByQuat2(float xoffset, float yoffset)

{

	//这个还是有问题的，因为求出来的四元数旋转轴是在世界空间的，会导致旋转一定角度后上下旋转会反过来，并且在Front接近WorldUp的时候会有天旋地转的感觉,所以这里我用/**/注释掉，更换新的标蓝色的

	/*

	// Find the rotation Between the front of the object (that we assume towards +Z,

	// but this depends on your model) and the desired direction

	glm::quat rot1 = RotationBetweenVectors(glm::vec3(0.0f, 0.0f, 1.0f),normalize(glm::vec3(0.008f * MouseSensitivity*xoffset, 0.008f * MouseSensitivity * yoffset, 1.0f)) );



	Front = glm::normalize(glm::rotate(rot1, Front));

	Right = glm::normalize(glm::cross(Front, WorldUp)); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.

	Up = glm::normalize(glm::cross(Right, Front));

	*/

	glm::vec3 ViewDest = Front + Right * xoffset * 0.008f * MouseSensitivity + Up * yoffset * 0.008f * MouseSensitivity;

	glm::quat rot1 = RotationBetweenVectors(Front, ViewDest);

	Front = glm::normalize(glm::rotate(rot1, Front));

	Right = glm::normalize(glm::cross(Front, Up));

	Up = glm::normalize(glm::cross(Right, Front));




}



/// <summary>

/// get rotation Between two vectors

/// </summary>

/// <param name="start"></param>

/// <param name="dest"></param>

/// <returns></returns>

glm::quat camera2::RotationBetweenVectors(glm::vec3 start, glm::vec3 dest)

{

	start = normalize(start);

	dest = normalize(dest);

	glm::vec3 rotateAxis = cross(start, dest);

	float Theta_half =glm::acos( dot(start, dest))/2.0f;




	
	return glm::quat(cos(Theta_half), rotateAxis.x * sin(Theta_half), rotateAxis.y * sin(Theta_half), rotateAxis.z * sin(Theta_half));


}



void camera2::ResetYawAndPitch()

{

	Yaw = YAW;

	Pitch = PITCH;

}


