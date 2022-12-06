#pragma once

#include <raylib.h>

#define NORM(LO, X, HI) (X - LO / (HI - LO))

struct Cam
{
	// default fields
	Camera3D rlcam = {
		{0,0,0}, // position
		{0,0,0}, // target
		{0,1,0}, // up dir
		90,      // fov
		CAMERA_PERSPECTIVE
	};
	
	// rotation
	float ry = 0;
	float rx = 0;
	float rz = 0;
	
	// rotation delta
	float dx = 0;
	float dy = 0;
	
	// target rotation delta
	float dxTarget = 0;
	float dyTarget = 0;
	
	// SPEED
	// ------
	// Amount of rotation per frame
	// 1   = 0.02 degrees
	// 5   = 0.1 degrees
	// 50  = 1 degrees
	// 100 = 2 degrees
	int speedX = 10;
	int speedY = 10;
	
	// SENSITIVITY
	// ------------
	// How quickly the camera
	// responds to input
	// 1     floaty
	// 10    loose
	// 30    natural
	// 50    fast
	// 70    tight
	// 90    precise
	int sensX = 40;
	int sensY = 40;
	
	// SMOOTHING
	// ---------
	// Smooths out quick fluctuations
	// 0    none
	// 10   rigid
	// 30   natural
	// 50   smooth
	// 70   fluid
	// 90   drunk
	int smoothX = 60;
	int smoothY = 60;
	
	vec3 position = {0,0,0};
	
	int orbit = true;
	float orbitDistance = 1;
	vec3 orbitOffset = {0,0,0};
	
	float lerpAverage(float from, float to, float weight);
	
	void rotate(float dy, float dx, float dz);
	void update();
	
	void calc(float& rotation, float& delta, float& deltaTarget, float total, int speed, int sens, int smooth); 
};

float Cam::lerpAverage(float from, float to, float weight = 0.5)
{
	return (from * (1-weight)) + (to * (weight));
}

void Cam::calc(float& r, float& dr, float& tdr, float t, int speed, int sens, int smooth)
{
	#define FAC(X, HI) ((float)CLAMP(1, X, HI) / (float)HI)
	
	// calculate interpolation weights
	float speedWeight = (float)speed / 50;
	float sensWeight = FAC (sens, 100);
	float smoothWeight = FAC (100 - smooth, 100);
	
	float target = t * DEG2RAD * speedWeight; // convert to radians for raylib calculations
	
	tdr = lerpAverage(tdr, t, sensWeight);
	dr = lerpAverage(dr, tdr, smoothWeight);
	r = fmod(r + dr, 2*M_PI);
	
	#undef FAC
}

void Cam::rotate(float y, float x, float z)
{
	test_input:
	{
		// DEBUG //
		if (IsKeyPressed(kup))
		{
			sensX = MIN(sensX + 5, 100);
			sensY = MIN(sensY + 5, 100);
			printf("sens: %i\n", sensX); 
		}
		else if (IsKeyPressed(kdown))
		{
			sensX = MAX(sensX - 5, 0);
			sensY = MAX(sensY - 5, 0);
			printf("sens: %i\n", sensX); 
		}
		
		// DEBUG //
		if (IsKeyPressed(kleft))
		{
			smoothX = MAX(smoothX - 5, 0);
			smoothY = MAX(smoothY - 5, 0);
			printf("smooth: %i\n", smoothX); 
		}
		else if (IsKeyPressed(kright))
		{
			smoothX = MIN(smoothX + 5, 100);
			smoothY = MIN(smoothY + 5, 100);
			printf("smooth: %i\n", smoothX); 
		}
	}
	
	calc(rx, dx, dxTarget, x, speedX, sensX, smoothX);
	calc(ry, dy, dyTarget, y, speedY, sensY, smoothY);
	
	ry = CLAMP((float)-1.55, ry, (float)1.55);
	rz = fmod(rx + z, 2*M_PI);
}

void Cam::update()
{
	Quaternion rotation =
		QuaternionFromEuler(ry, rx, rz);
	
	vec3 lookdir =
		Vector3RotateByQuaternion({0,0,-1}, rotation);
	rlcam.target = Vector3Add(position, lookdir);
	
	rlcam.position = position;
	
	if (orbit)
	{
		lookdir = Vector3Scale(lookdir, orbitDistance);
		rlcam.position = Vector3Subtract(rlcam.position, lookdir);
		rlcam.position = Vector3Add(rlcam.position, orbitOffset);
	}
}
