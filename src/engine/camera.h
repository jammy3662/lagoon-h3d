#pragma once

#include <raylib.h>

#define Camera _Camera

struct Camera: public virtual Camera3D
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
	
	Camera()
	{
		SetCameraMode((Camera3D)*this, CAMERA_CUSTOM);
		up = {0,1,0};
		
		nearClip = 0.01;
		farClip = 1000.0;
		
		pitch = 0;
		yaw = 0;
		roll = 0;
	}
	
	void refresh();
	void rotate(float y, float x, float z);
};

void Camera::refresh()
{
	#define vrq Vector3RotateByQuaternion
	
	Quaternion rotation = QuaternionFromEuler(pitch, yaw, roll);
	
	vec3 lookdir = vrq({0,0,-1}, rotation);
	vec3 backdir = vrq({0,0,orbitDist}, rotation);
	
	target = Vector3Add(anchor, lookdir);
	position = anchor;
	
	if (orbit)
	{
		position = Vector3Subtract(anchor, backdir);
		position = Vector3Add(position, orbitOffset);
	}
	
	#undef vrq
}

void Camera::rotate(float y, float x, float z)
{
	pitch = clamp((float)-1.55, pitch + y, (float)1.55);
	yaw = fmod(yaw + x, 2*M_PI);
	roll = fmod(roll + z, 2*M_PI);
}