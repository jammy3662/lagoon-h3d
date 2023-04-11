#pragma once

// custom (more crude) bool type
#define bool char
#define false 0
#define true 1

typedef struct{float x,y;} vec2;
typedef struct{float x,y,z;} vec3;
typedef struct{float x,y,z,w;} vec4;

float waverage(float from, float to, float weight) {
	return (from * (1-weight)) + (to * (weight));
}

vec2 waverage(vec2 from, vec2 to, float weight) {
	return vec2 {
		waverage(from.x, to.x, weight),
		waverage(from.y, to.y, weight)
	};
}

namespace Input
{
	enum ButtonField
	{
		ACCEPT = 0x01,
		CANCEL = 0x02,
		INFO = 0x04,
		SPECIAL = 0x08,
		MENU = 0x10,

		PICK = 0x80,
		SHIFT = 0x20,
		ALT = 0x40,
		
		MORE = 0x100,
		LESS = 0x200,
		PREV = 0x400,
		NEXT = 0x800
	};
	
	ButtonField pressed;
	ButtonField held;
	
	inline bool isPressed(ButtonField button) {
		return (pressed & button);
	}
	inline bool isHeld(ButtonField button)	{
		return (held &	button);
	}
	
	vec2 joystick;
	vec2 pointer;
}