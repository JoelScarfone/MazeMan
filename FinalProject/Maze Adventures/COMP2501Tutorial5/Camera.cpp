#include "Camera.h"
#include <iostream>

Camera::Camera(Vector3 pos, Vector3 lookat, Vector3 up)
{
	position = pos;
	playerForwardVector = lookat - pos;
	cameraForwardVector = lookat - pos;

	cameraForwardVector.normalize();
	playerForwardVector.normalize();
	
	playerUpVector = up;
	cameraUpVector = up;

	playerUpVector.normalize();
	cameraUpVector.normalize();
}

Vector3 Camera::getForward(){
	return playerForwardVector;
}
void Camera::pitch(float angleDeg){
	updateOrientation(Vector3::cross(playerForwardVector, playerUpVector), angleDeg);
}

void Camera::yaw(float angleDeg){
	updateOrientation(playerUpVector, angleDeg);
	//updateOrientation(Vector3(0,1,0), angleDeg);	//bonus
}

void Camera::move(float distance){
	position += playerForwardVector * distance;
	//position.y = 2;
}

void Camera::strafe(float distance){
	position += Vector3::cross(playerForwardVector, playerUpVector) * distance;
	//position.y = 2;
}

void Camera::climb(float distance){
	position += playerUpVector * distance;
}
	
void Camera::jump(float height){
	float floatTime = 30;
	float fastFall = 5;
	//std::cout << height - gravity << std::endl;
	jumpSpeed = height*fastFall - gravity;
	gravity += gravity / floatTime;
	position += playerUpVector*jumpSpeed;
}
void Camera::updateOrientation(Vector3 axis, float angleDeg){
	Matrix4 rotMatrix = Matrix4::IDENTITY;
	rotMatrix.rotate(axis, angleDeg);

	cameraUpVector = cameraUpVector * rotMatrix;
	cameraForwardVector = cameraForwardVector * rotMatrix;

	playerForwardVector.x = cameraForwardVector.x;
	playerForwardVector.z = cameraForwardVector.z;

	playerForwardVector.normalize();
	cameraUpVector.normalize();
	cameraForwardVector.normalize();
}

Matrix4 Camera::getViewMatrix(){
	return createViewMatrix(position, cameraForwardVector, cameraUpVector);
}


void Camera::mouseLook(float angleX, float angleZ){
	yaw(angleX);
	pitch(angleZ);
	if (cameraUpVector.y < .65){
		pitch(-angleZ);
	}
}