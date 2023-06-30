#pragma once

#include "define.h"
#include "backend.h"

#include <array>

enum InputDevice
{
	KEYBOARD,
	CLICK,
	GAMEPAD,
	MOUSE_MOVE, // cursor movement
	MOUSE_SCROLL, // scroll wheel
	JOYSTICK,
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
	
	// left joystick only
	AXIS_NORTH,
	AXIS_SOUTH,
	AXIS_WEST,
	AXIS_EAST,
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

typedef std::array <zInputScalar, INPUT_ACTION_CT> Mapping;

Mapping mouseAndKeyboard =
{{
	{.device = KEYBOARD, .code = GLFW_KEY_SPACE},
	{.device = KEYBOARD, .code = GLFW_KEY_F},
	{.device = KEYBOARD, .code = GLFW_KEY_TAB},
	{.device = KEYBOARD, .code = GLFW_KEY_Q},
	
	{.device = KEYBOARD, .code = GLFW_KEY_ESCAPE},
	
	{.device = CLICK, .code = MOUSE_LEFT},
	{.device = CLICK, .code = MOUSE_RIGHT},
	{.device = KEYBOARD, .code = GLFW_KEY_LEFT_SHIFT},
	
	{.device = KEYBOARD, .code = GLFW_KEY_2},
	{.device = KEYBOARD, .code = GLFW_KEY_3},
	{.device = KEYBOARD, .code = GLFW_KEY_4},
	{.device = KEYBOARD, .code = GLFW_KEY_1},
	
	{.device = KEYBOARD, .code = GLFW_KEY_W},
	{.device = KEYBOARD, .code = GLFW_KEY_S},
	{.device = KEYBOARD, .code = GLFW_KEY_A},
	{.device = KEYBOARD, .code = GLFW_KEY_D},
	
	{.device = MOUSE_MOVE, .code = AXIS_UP},
	{.device = MOUSE_MOVE, .code = AXIS_DOWN},
	{.device = MOUSE_MOVE, .code = AXIS_LEFT},
	{.device = MOUSE_MOVE, .code = AXIS_RIGHT},
}};

Mapping gamepadAndJoystick =
{{
	{.device = GAMEPAD, .code = A_BUT},
	{.device = GAMEPAD, .code = B_BUT},
	{.device = GAMEPAD, .code = X_BUT},
	{.device = GAMEPAD, .code = Y_BUT},
	
	{.device = GAMEPAD, .code = START_BUT},
	
	{.device = GAMEPAD, .code = ZR_BUT},
	{.device = GAMEPAD, .code = R_BUT},
	{.device = GAMEPAD, .code = ZL_BUT},
	
	{.device = GAMEPAD, .code = UP_BUT},
	{.device = GAMEPAD, .code = DOWN_BUT},
	{.device = GAMEPAD, .code = LEFT_BUT},
	{.device = GAMEPAD, .code = RIGHT_BUT},
	
	{.device = JOYSTICK, .code = AXIS_NORTH},
	{.device = JOYSTICK, .code = AXIS_SOUTH},
	{.device = JOYSTICK, .code = AXIS_WEST},
	{.device = JOYSTICK, .code = AXIS_RIGHT},
	
	{.device = JOYSTICK, .code = AXIS_UP},
	{.device = JOYSTICK, .code = AXIS_DOWN},
	{.device = JOYSTICK, .code = AXIS_LEFT},
	{.device = JOYSTICK, .code = AXIS_RIGHT},
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
	return (input.pressedActions [button]);
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
	
	glfwGetGamepadState (input.gamepadIdx, & input.gamepad);
	
	// retrieve cursor data
	{
		static double x, y;
		glfwGetCursorPos (input.window, &x, &y);
		
		input.mouseX = x - input.lastMouseX;
		input.lastMouseX = x;
		
		input.mouseY = y - input.lastMouseY;
		input.lastMouseY = y;
	}
	
	// store state from last frame
	input.heldActions = input.pressedActions;
	// clean state to read next inputs
	input.pressedActions.fill (false);
	
	int idx = 0;
	
	// update each binding as per configuration
	for (zInputScalar& binding : input.mapping)
	{
		binding.strength = 0.0;
		
		switch (binding.device)
		{
			int button;
			case KEYBOARD:
				button = glfwGetKey (input.window, binding.code);
				if (button == GLFW_PRESS) binding.strength = 1.0;
			break;
			
			case GAMEPAD:
				if (binding.code == ZL_BUT)
					binding.strength = input.gamepad.axes [GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
				else if (binding.code == ZR_BUT)
					binding.strength = input.gamepad.axes [GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];
				else if (input.gamepad.buttons [binding.code])
					binding.strength = 1.0;
			break;
			
			case CLICK:
				button = glfwGetMouseButton (input.window, binding.code);
				if (button == GLFW_PRESS) binding.strength = 1.0;
			break;
			
			case MOUSE_MOVE:
				if (binding.code == AXIS_UP)
					binding.strength = abs (MIN (input.mouseY, 0));
				else if (binding.code == AXIS_DOWN)
					binding.strength = MAX (0, input.mouseY);
				else if (binding.code == AXIS_LEFT)
					binding.strength = abs (MIN (input.mouseX, 0));
				else if (binding.code == AXIS_RIGHT)
					binding.strength = MAX (input.mouseX, 0);
			break;
			
			case MOUSE_SCROLL:
				if (binding.code == AXIS_UP)
					binding.strength = abs (MIN (input.glfwScrollY, 0));
				else if (binding.code == AXIS_DOWN)
					binding.strength = MAX (0, input.glfwScrollY);
				else if (binding.code == AXIS_LEFT)
					binding.strength = abs (MIN (input.glfwScrollX, 0));
				else if (binding.code == AXIS_RIGHT)
					binding.strength = MAX (input.glfwScrollX, 0);
			break;
			
			case JOYSTICK:
				if (binding.code == AXIS_UP)
					binding.strength = abs (MIN (input.mouseY, 0));
				else if (binding.code == AXIS_DOWN)
					binding.strength = MAX (0, input.mouseY);
				else if (binding.code == AXIS_LEFT)
					binding.strength = abs (MIN (input.mouseX, 0));
				else if (binding.code == AXIS_RIGHT)
					binding.strength = MAX (input.mouseX, 0);
				
				if (binding.code == AXIS_NORTH)
					binding.strength = abs (MIN (input.gamepad.axes [GLFW_GAMEPAD_AXIS_LEFT_Y], 0));
				else if (binding.code == AXIS_SOUTH)
					binding.strength = MAX (0, input.gamepad.axes [GLFW_GAMEPAD_AXIS_LEFT_Y]);
				else if (binding.code == AXIS_WEST)
					binding.strength = abs (MIN (input.gamepad.axes [GLFW_GAMEPAD_AXIS_LEFT_X], 0));
				else if (binding.code == AXIS_EAST)
					binding.strength = MAX (input.gamepad.axes [GLFW_GAMEPAD_AXIS_LEFT_X], 0);
			break;
		}
		
		input.pressedActions [idx] = (binding.strength != 0.0);
		
		#ifdef DEBUG_INPUT
		
		if (getButtonNow ((InputAction) idx))
			printf ("◌ %s\n", inputNames [idx]);
		
		#endif
		
		idx++;
	}
}

