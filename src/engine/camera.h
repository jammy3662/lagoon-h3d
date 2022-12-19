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
int speedX = 40;
int speedY = 40;

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
int sens = 80;

// SMOOTHING
// ---------
// Smooths out quick fluctuations
// 0   none
// 10  rigid
// 30  natural
// 50  smooth
// 70  fluid
// 90  drunk
int smooth = 70;

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
			sens = MIN(sens + 5, 100);
			printf("sens: %i\n", sens); 
		}
		else if (IsKeyPressed(kdown))
		{
			sens = MAX(sens - 5, 0);
			printf("sens: %i\n", sens); 
		}
		
		// DEBUG //
		if (IsKeyPressed(kleft))
		{
			smooth = MAX(smooth - 5, 0);
			printf("smooth: %i\n", smooth); 
		}
		else if (IsKeyPressed(kright))
		{
			smooth = MIN(smooth + 5, 100);
			printf("smooth: %i\n", smooth); 
		}
	}
	
	calc(rx, x, dxTarget, dx, speedX, sens, smooth);
	calc(ry, y, dyTarget, dy, speedY, sens, smooth);
	
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
