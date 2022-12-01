#pragma once

#include <raylib.h>

#define Camera _Camera
#define rlCamera Camera3D

struct Camera: public rlCamera
{
	// horizontal and vertical
	// camera sensitivity
	float HSENS;
	float VSENS;
	
	float nearClip;
	float farClip;
	
	// in radians
	float pitch;
	float yaw;
	float roll;
	
	// third person camera
	int orbit;
	float orbitDist; // distance from camera to orbit point
	vec3 orbitOffset; // distance from camera target to orbit point
	
	vec3 anchor; // pov-agnostic position of camera
	
	void init();
	void refresh();
	void rotate(float y, float x, float z);
};

void Camera::init()
{
	up = {0,1,0};
	SetCameraMode((Camera3D)*this, CAMERA_CUSTOM);
}

void Camera::refresh()
{
	Quaternion rotation = QuaternionFromEuler(pitch, yaw, roll);
	
	// position + looking direction
	target = Vector3Add
	(
		anchor,
		Vector3RotateByQuaternion
			({0,0,1}, rotation)
	);
	
	if (orbit)
	{
		// position - looking direction * orbit distance
		position = Vector3Subtract
		(
			anchor,
			Vector3RotateByQuaternion
				({0,0,orbitDist * orbit}, rotation)
		);
		position = Vector3Add(position, orbitOffset);
	}
	else position = anchor;
}

void Camera::rotate(float y, float x, float z)
{
	pitch = clamp((float)-1.55, pitch + y, (float)1.55);
	yaw = fmod(yaw + x, 2*M_PI);
	roll = fmod(roll + z, 2*M_PI);
	
	pitch = clamp((float)-1.55, pitch, (float)1.55);
	
	refresh();
}