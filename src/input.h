#pragma once

#include "define.h"
#include "backend.h"

#include <array>

enum InputDevice
{
	BUTTON_KEY,
	BUTTON_MOUSE,
	BUTTON_GAMEPAD,
	ANALOG_MOUSE, // cursor movement
	ANALOG_SCROLL, // scroll wheel
	ANALOG_JOYSTICK_L,
	ANALOG_JOYSTICK_R
};

enum MouseCode
{
	MOUSE_LEFT = GLFW_MOUSE_BUTTON_LEFT,
	MOUSE_RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
	MOUSE_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE,
};

enum GamepadCode
{
	A_BUT = GLFW_GAMEPAD_BUTTON_A,
	B_BUT = GLFW_GAMEPAD_BUTTON_B,
	X_BUT = GLFW_GAMEPAD_BUTTON_X,
	Y_BUT = GLFW_GAMEPAD_BUTTON_Y,
	
	L_BUT = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
	R_BUT = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
	ZL_BUT,
	ZR_BUT,
	
	UP_BUT = GLFW_GAMEPAD_BUTTON_DPAD_UP,
	DOWN_BUT = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
	LEFT_BUT = GLFW_GAMEPAD_BUTTON_DPAD_LEFT, 
	RIGHT_BUT = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
	
	START_BUT = GLFW_GAMEPAD_BUTTON_START,
};

enum AnalogCode
{
	AXIS_UP,
	AXIS_DOWN,
	AXIS_LEFT,
	AXIS_RIGHT,
};

enum InputAction
{
	ACCEPT,
	CANCEL,
	INFO,
	SPECIAL,
	
	MENU,
	
	PICK,
	ALT,
	SHIFT,
	
	MORE,
	LESS,
	PREV,
	NEXT,
	
	NAV_UP,
	NAV_DOWN,
	NAV_LEFT,
	NAV_RIGHT,
	
	POINT_UP,
	POINT_DOWN,
	POINT_LEFT,
	POINT_RIGHT,
	
	INPUT_ACTION_CT
};

#ifdef DEBUG_INPUT

const char * inputNames [] =
{
	"accept", "cancel", "info", "special",
	"menu", "pick", "alt", "shift",
	"more", "less", "prev", "next",
	"nav up", "nav down", "nav left", "nav right",
	"point up", "point down", "point left", "point right",
};

#endif

struct zInputScalar
{
	int device;
	int code;
	double strength;
};

struct zInputVector
{
	zInputScalar down, up;
	zInputScalar left, right;
};

typedef std::array <zInputScalar, INPUT_ACTION_CT> Mapping;

Mapping mouseAndKeyboard =
{{
	{.device = BUTTON_KEY, .code = GLFW_KEY_SPACE},
	{.device = BUTTON_KEY, .code = GLFW_KEY_F},
	{.device = BUTTON_KEY, .code = GLFW_KEY_TAB},
	{.device = BUTTON_KEY, .code = GLFW_KEY_Q},
	
	{.device = BUTTON_KEY, .code = GLFW_KEY_ESCAPE},
	
	{.device = BUTTON_MOUSE, .code = MOUSE_LEFT},
	{.device = BUTTON_MOUSE, .code = MOUSE_RIGHT},
	{.device = BUTTON_KEY, .code = GLFW_KEY_LEFT_SHIFT},
	
	{.device = BUTTON_KEY, .code = GLFW_KEY_2},
	{.device = BUTTON_KEY, .code = GLFW_KEY_3},
	{.device = BUTTON_KEY, .code = GLFW_KEY_4},
	{.device = BUTTON_KEY, .code = GLFW_KEY_1},
	
	{.device = BUTTON_KEY, .code = GLFW_KEY_W},
	{.device = BUTTON_KEY, .code = GLFW_KEY_S},
	{.device = BUTTON_KEY, .code = GLFW_KEY_A},
	{.device = BUTTON_KEY, .code = GLFW_KEY_D},
	
	{.device = ANALOG_MOUSE, .code = AXIS_UP},
	{.device = ANALOG_MOUSE, .code = AXIS_DOWN},
	{.device = ANALOG_MOUSE, .code = AXIS_LEFT},
	{.device = ANALOG_MOUSE, .code = AXIS_RIGHT},
}};

Mapping gamepadAndJoystick =
{{
	{.device = BUTTON_GAMEPAD, .code = A_BUT},
	{.device = BUTTON_GAMEPAD, .code = B_BUT},
	{.device = BUTTON_GAMEPAD, .code = X_BUT},
	{.device = BUTTON_GAMEPAD, .code = Y_BUT},
	
	{.device = BUTTON_GAMEPAD, .code = START_BUT},
	
	{.device = BUTTON_GAMEPAD, .code = ZR_BUT},
	{.device = BUTTON_GAMEPAD, .code = R_BUT},
	{.device = BUTTON_GAMEPAD, .code = ZL_BUT},
	
	{.device = BUTTON_GAMEPAD, .code = UP_BUT},
	{.device = BUTTON_GAMEPAD, .code = DOWN_BUT},
	{.device = BUTTON_GAMEPAD, .code = LEFT_BUT},
	{.device = BUTTON_GAMEPAD, .code = RIGHT_BUT},
	
	{.device = ANALOG_JOYSTICK_L, .code = AXIS_UP},
	{.device = ANALOG_JOYSTICK_L, .code = AXIS_DOWN},
	{.device = ANALOG_JOYSTICK_L, .code = AXIS_LEFT},
	{.device = ANALOG_JOYSTICK_L, .code = AXIS_RIGHT},
	
	{.device = ANALOG_JOYSTICK_R, .code = AXIS_UP},
	{.device = ANALOG_JOYSTICK_R, .code = AXIS_DOWN},
	{.device = ANALOG_JOYSTICK_R, .code = AXIS_LEFT},
	{.device = ANALOG_JOYSTICK_R, .code = AXIS_RIGHT},
}};

struct
{
	GLFWwindow* window;
	
	int gamepadIdx;
	GLFWgamepadstate gamepad;
	
	// relative movement
	double mouseX, mouseY;	
	double lastMouseX, lastMouseY;
	
	double glfwScrollX, glfwScrollY;
	
	Mapping mapping = mouseAndKeyboard;
	
	std::array <bool, INPUT_ACTION_CT> pressedActions;
	std::array <bool, INPUT_ACTION_CT> heldActions;
}
input;

// only way to access scroll input is through a callback
void zInputScrollCallback (GLFWwindow* window, double x, double y)
{
	input.glfwScrollX = x;
	input.glfwScrollY = y;
}

void releaseCursor ()
{
	glfwSetInputMode (input.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void captureCursor ()
{
	glfwSetInputMode (input.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

bool getButton (InputAction button)
{
	return (input.mapping [button].strength != 0);
}

bool getButtonNow (InputAction button)
{
	return (input.pressedActions [button] && ! input.heldActions [button]);
}

vec2 getAxis1 ()
{
	return vec2
	{
		input.mapping [NAV_RIGHT].strength - input.mapping [NAV_LEFT].strength,
		input.mapping [NAV_UP].strength - input.mapping [NAV_DOWN].strength
	};
}

vec2 getAxis2 ()
{
	return vec2
	{
		input.mapping [POINT_RIGHT].strength - input.mapping [POINT_LEFT].strength,
		input.mapping [POINT_UP].strength - input.mapping [POINT_DOWN].strength
	};
}

void inputInit ()
{
	input.window = gpu.window;
	
	// TODO: load saved controls from file
	
	glfwSetScrollCallback (input.window, zInputScrollCallback);
	
	if (glfwRawMouseMotionSupported ())
	{
		glfwSetInputMode (input.window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
	
	glfwSetInputMode (input.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode (input.window, GLFW_STICKY_KEYS, GLFW_TRUE);
}

void inputUpdate ()
{
	glfwPollEvents ();
	
	// retrieve cursor data
	static double mx, my;
	glfwGetCursorPos (input.window, &mx, &my);
	input.mouseX = mx - input.lastMouseX;
	input.mouseY = my - input.lastMouseY;
	input.lastMouseX = mx;
	input.lastMouseY = my;
	
	// store state from last frame
	input.heldActions = input.pressedActions;
	// clean state to read next inputs
	input.pressedActions.fill (false);
	
	int idx = 0;
	
	// update each binding as per configuration
	for (zInputScalar& binding : input.mapping)
	{
		binding.strength = 0.0;
		
		static int button;
		static double axisX, axisY;
		
		switch (binding.device)
		{
			case BUTTON_KEY:
				button = glfwGetKey (input.window, binding.code);
				if (button == GLFW_PRESS) binding.strength = 1.0;
			break;
			
			case BUTTON_GAMEPAD:
				glfwGetGamepadState (input.gamepadIdx, & input.gamepad);
				if (binding.code == ZL_BUT)
				{
					binding.strength = input.gamepad.axes [GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
					break;
				}
				if (binding.code == ZR_BUT)
				{
					binding.strength = input.gamepad.axes [GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];
					break;
				}
				if (input.gamepad.buttons [binding.code])
					binding.strength = 1.0;
			break;
			
			case BUTTON_MOUSE:
				button = glfwGetMouseButton (input.window, binding.code);
				if (button == GLFW_PRESS) binding.strength = 1.0;
			break;
			
			case ANALOG_MOUSE:
				axisX = input.mouseX;
				axisY = input.mouseY;
			break;
			case ANALOG_SCROLL:
				axisX = input.glfwScrollX;
				axisY = input.glfwScrollY;
			break;
			case ANALOG_JOYSTICK_L:
				axisX = input.gamepad.axes [GLFW_GAMEPAD_AXIS_LEFT_X];
				axisY = input.gamepad.axes [GLFW_GAMEPAD_AXIS_LEFT_Y];
			break;
			case ANALOG_JOYSTICK_R:
				axisX = input.gamepad.axes [GLFW_GAMEPAD_AXIS_RIGHT_X];
				axisY = input.gamepad.axes [GLFW_GAMEPAD_AXIS_RIGHT_Y];
			break;
		}
		
		if (binding.device == ANALOG_MOUSE || binding.device == ANALOG_SCROLL ||
			binding.device == ANALOG_JOYSTICK_L || binding.device == ANALOG_JOYSTICK_R)
		{
				switch (binding.code)
				{
					case AXIS_UP:
						if (axisY >	0) binding.strength = axisY;
					break;
					case AXIS_DOWN:
						if (axisY < 0) binding.strength = -axisY;
					break;
					case AXIS_LEFT:
						if (axisX < 0) binding.strength = -axisX;
					break;
					case AXIS_RIGHT:
						if (axisX >	0) binding.strength = axisX;
					break;
				}
		}
		
		input.pressedActions [idx] = (binding.strength != 0.0);
		
		#ifdef DEBUG_INPUT
		
		if (getButtonNow ((InputAction) idx))
			printf ("◌ %s\n", inputNames [idx]);
		
		#endif
		
		idx++;
	}
}

