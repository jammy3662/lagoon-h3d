#pragma once

#include <raylib.h>

struct Cam : public virtual Camera3D {

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
// 0     none
// 5     1/100 degree
// 50    1/10 degree
// 100   MAX
// 500   1 degree
int speedX = 50;
int speedY = 50;

// SENSITIVITY
// ------------
// How quickly the camera
// responds to input
// 1    floaty
// 10   loose
// 30   natural
// 50   fast
// 70   tight
// 90   precise
int sensX = 80;
int sensY = 80;

// SMOOTHING
// ---------
// Smooths out quick fluctuations
// 0   none
// 10  rigid
// 30  natural
// 50  smooth
// 70  fluid
// 90  drunk
int smoothX = 70;
int smoothY = 70;

vec3 anchor = {0,0,0};

vec3 lookdir = {0,0,-1};

int orbit = false;
float orbitDistance = 1;
vec3 orbitOffset = {0,0,0};

float lerpAverage(float from, float to, float weight);

void rotate(float y, float x, float z)
{
	// DEBUG:
	// up/down - change sensitivity
	// left/right - change smoothing
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
	
	calc(rx, x, dxTarget, dx, speedX, sensX, smoothX);
	calc(ry, y, dyTarget, dy, speedY, sensY, smoothY);
	
	ry = CLAMP((float)-1.55, ry, (float)1.55);
	rz = fmod(rz + z, 2*M_PI);
}

void update()
{
	Quaternion rotation =
		QuaternionFromEuler(ry, rx, rz);
	
	lookdir =
		Vector3RotateByQuaternion({0,0,-1}, rotation);
	target = Vector3Add(anchor, lookdir);
	
	position = anchor;
	
	//if (orbit)
	//{
		vec3 reach = Vector3Scale(lookdir, orbitDistance);
		position = Vector3Subtract(position, reach);
		position = Vector3Add (
			position,
			Vector3RotateByQuaternion(orbitOffset, rotation)
		);
		target = Vector3Add (
			target,
			Vector3RotateByQuaternion(orbitOffset, rotation)
		);
	//}
}

void calc(float& rotation, float& delta, float& delta2, float& delta3, int speed, int sens, int smooth)
{
	#define FAC(X, HI) ((float)CLAMP(1, X, HI) / (float)HI)
	
	// calculate interpolation weights
	float speedWeight = (float)speed / 500;
	float sensWeight = FAC (sens, 100);
	float smoothWeight = FAC (100 - smooth, 100);
	
	delta *= speedWeight; // rotation speed
	delta2 = waverage(delta2, delta, sensWeight);
	delta3 = waverage(delta3, delta2, smoothWeight);
	
	rotation = fmod(rotation + delta3, 2*M_PI);
	
	#undef FAC
}

}; // Cam

float Cam::lerpAverage(float from, float to, float weight = 0.5)
{
	return (from * (1-weight)) + (to * (weight));
}

