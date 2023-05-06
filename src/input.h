#pragma once

#include "define.h"

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
	MOUSE_RIGHT = GLFW_MOUSE_BUTTON_RIGHT
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

struct InputScalar
{
	int device;
	int code;
	double strength;
};

struct InputVector
{
	InputScalar down, up;
	InputScalar left, right;
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

typedef InputScalar Mapping [INPUT_ACTION_CT];

Mapping mouseAndKeyboard =
{
	InputScalar {.device = BUTTON_KEY, .code = GLFW_KEY_SPACE},
	InputScalar {.device = BUTTON_KEY, .code = GLFW_KEY_F},
	InputScalar {.device = BUTTON_KEY, .code = GLFW_KEY_TAB},
	InputScalar {.device = BUTTON_KEY, .code = GLFW_KEY_Q},
	
	InputScalar {.device = BUTTON_KEY, .code = GLFW_KEY_ESCAPE},
	
	InputScalar {.device = BUTTON_MOUSE, .code = MOUSE_LEFT},
	InputScalar {.device = BUTTON_MOUSE, .code = MOUSE_RIGHT},
	InputScalar {.device = BUTTON_KEY, .code = GLFW_KEY_LEFT_SHIFT},
	
	InputScalar {.device = BUTTON_KEY, .code = GLFW_KEY_2},
	InputScalar {.device = BUTTON_KEY, .code = GLFW_KEY_3},
	InputScalar {.device = BUTTON_KEY, .code = GLFW_KEY_4},
	InputScalar {.device = BUTTON_KEY, .code = GLFW_KEY_1},
	
	InputScalar {.device = BUTTON_KEY, .code = GLFW_KEY_W},
	InputScalar {.device = BUTTON_KEY, .code = GLFW_KEY_S},
	InputScalar {.device = BUTTON_KEY, .code = GLFW_KEY_A},
	InputScalar {.device = BUTTON_KEY, .code = GLFW_KEY_D},
	
	InputScalar {.device = ANALOG_MOUSE, .code = AXIS_UP},
	InputScalar {.device = ANALOG_MOUSE, .code = AXIS_DOWN},
	InputScalar {.device = ANALOG_MOUSE, .code = AXIS_LEFT},
	InputScalar {.device = ANALOG_MOUSE, .code = AXIS_RIGHT},
};

Mapping gamepadAndJoystick =
{
	InputScalar {.device = BUTTON_GAMEPAD, .code = A_BUT},
	InputScalar {.device = BUTTON_GAMEPAD, .code = B_BUT},
	InputScalar {.device = BUTTON_GAMEPAD, .code = X_BUT},
	InputScalar {.device = BUTTON_GAMEPAD, .code = Y_BUT},
	
	InputScalar {.device = BUTTON_GAMEPAD, .code = START_BUT},
	
	InputScalar {.device = BUTTON_GAMEPAD, .code = ZR_BUT},
	InputScalar {.device = BUTTON_GAMEPAD, .code = R_BUT},
	InputScalar {.device = BUTTON_GAMEPAD, .code = ZL_BUT},
	
	InputScalar {.device = BUTTON_GAMEPAD, .code = UP_BUT},
	InputScalar {.device = BUTTON_GAMEPAD, .code = DOWN_BUT},
	InputScalar {.device = BUTTON_GAMEPAD, .code = LEFT_BUT},
	InputScalar {.device = BUTTON_GAMEPAD, .code = RIGHT_BUT},
	
	InputScalar {.device = ANALOG_JOYSTICK_L, .code = AXIS_UP},
	InputScalar {.device = ANALOG_JOYSTICK_L, .code = AXIS_DOWN},
	InputScalar {.device = ANALOG_JOYSTICK_L, .code = AXIS_LEFT},
	InputScalar {.device = ANALOG_JOYSTICK_L, .code = AXIS_RIGHT},
	
	InputScalar {.device = ANALOG_JOYSTICK_R, .code = AXIS_UP},
	InputScalar {.device = ANALOG_JOYSTICK_R, .code = AXIS_DOWN},
	InputScalar {.device = ANALOG_JOYSTICK_R, .code = AXIS_LEFT},
	InputScalar {.device = ANALOG_JOYSTICK_R, .code = AXIS_RIGHT},
};

struct InputContext
{
	GLFWwindow * window;
	
	int gamepadIdx = 0;
	GLFWgamepadstate gamepad;
	
	// relative movement
	double mouseX, mouseY;
	
	double lastMouseX, lastMouseY;
	
	Mapping mapping;
	
	long pressedActions;
	long heldActions;
};

double glfwScrollX;
double glfwScrollY;

void inputScrollCallback (GLFWwindow *	window, double x, double y)
{
	glfwScrollX = x;
	glfwScrollY = y;
}

InputContext connectInput (GLFWwindow * window)
{
	InputContext ret = (InputContext) {0};
	
	ret.window = window;
	
	// set default controls (keyboard and mouse)
	for (int i = 0; i < INPUT_ACTION_CT; i++)
	{
		ret.mapping [i] = mouseAndKeyboard [i];
	}
	
	glfwSetScrollCallback (window, inputScrollCallback);
	
	if (glfwRawMouseMotionSupported ())
	{
		glfwSetInputMode (ret.window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
	
	glfwSetInputMode (ret.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode (ret.window, GLFW_STICKY_KEYS, GLFW_TRUE);
	
	return ret;
}

void updateInputs (InputContext & input)
{
	glfwPollEvents ();
	
	static double mx, my;
	glfwGetCursorPos (input.window, & mx, & my);
	input.mouseX = mx - input.lastMouseX;
	input.lastMouseX = mx;
	input.mouseY = my - input.lastMouseY;
	input.lastMouseY = my;
	
	// store state from last frame
	input.heldActions = input.pressedActions;
	input.pressedActions = 0;
	
	for (int i = 0; i < INPUT_ACTION_CT; i++)
	{
		InputScalar & binding = input.mapping [i];
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
				axisX = glfwScrollX;
				axisY = glfwScrollY;
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
		
		if (binding.device == ANALOG_MOUSE || 
			binding.device == ANALOG_SCROLL ||
			binding.device == ANALOG_JOYSTICK_L ||
			binding.device == ANALOG_JOYSTICK_R)
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
		
		if (binding.strength != 0.0)
			input.pressedActions |= 1 << i;
	}
}

bool getButton (InputContext & input, InputAction button)
{
	return (input.mapping [button].strength != 0);
}

bool getButtonNow (InputContext & input, InputAction button)
{
	long mask = 1 << button;
	return
	(
		mask & input.pressedActions &&
		!(mask &	input.heldActions)
	);
}

vec2 getNav (InputContext & input)
{
	return vec2
	{
		input.mapping [NAV_RIGHT].strength - input.mapping [NAV_LEFT].strength,
		input.mapping [NAV_UP].strength - input.mapping [NAV_DOWN].strength
	};
}

vec2 getPoint (InputContext & input)
{
	return vec2
	{
		input.mapping [POINT_RIGHT].strength - input.mapping [POINT_LEFT].strength,
		input.mapping [POINT_UP].strength - input.mapping [POINT_DOWN].strength
	};
}