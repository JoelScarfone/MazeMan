#pragma once

#include "MathHelper.h"

class Camera
{
public:
	Camera(Vector3 pos, Vector3 lookat, Vector3 up);
	void roll(float angleDeg);
	void pitch(float angleDeg);
	void yaw(float angleDeg);
	void move(float distance);
	void strafe(float distance);
	void climb(float distance);
	void jump(float height);
	float gravity = 0.25;
	float jumpSpeed = 0;
	Vector3 getForward();
	void setForward(Vector3 v){ cameraForwardVector = v; }
	Matrix4 getViewMatrix();
	void resetUpY();
	void setPosition(Vector3 _position) { position = _position; }
	Vector3 getCameraUp(){ return playerUpVector; };
	void setCameraUp(Vector3 v){ cameraUpVector = v; };
	Vector3 getPosition() { return position; };
	void mouseLook(float angleX, float angleZ);
	
protected:
	void updateOrientation(Vector3 axis, float angleDeg);

	Vector3 cameraUpVector;				//Camera Up
	Vector3 cameraForwardVector;			//Camera Forward

	Vector3 playerUpVector;			//Player Up
	Vector3 playerForwardVector;	// Player Forwar	

	Vector3 position;				// player and camera position

};