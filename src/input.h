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

// only way to access scroll input is through a callback
void zInputScrollCallback (GLFWwindow* window, double x, double y);

struct input
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
	
	void init ()
	{
		window = gpu.window;
		
		// TODO: load saved controls from file
		
		glfwSetScrollCallback (window, zInputScrollCallback);
		
		if (glfwRawMouseMotionSupported ())
		{
			glfwSetInputMode (window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		}
		
		glfwSetInputMode (window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetInputMode (window, GLFW_STICKY_KEYS, GLFW_TRUE);
	}
	
	void update ()
	{
		glfwPollEvents ();
		
		glfwGetGamepadState (gamepadIdx, &gamepad);
		
		// retrieve cursor data
		{
			static double x, y;
			glfwGetCursorPos (window, &x, &y);
			
			mouseX = x - lastMouseX;
			lastMouseX = x;
			
			mouseY = y - lastMouseY;
			lastMouseY = y;
		}
		
		// store state from last frame
		heldActions = pressedActions;
		// clean state to read next inputs
		pressedActions.fill (false);
		
		int idx = 0;
		
		// update each binding as per configuration
		for (zInputScalar& binding : mapping)
		{
			binding.strength = 0.0;
			
			switch (binding.device)
			{
				int button;
				case KEYBOARD:
					button = glfwGetKey (window, binding.code);
					if (button == GLFW_PRESS) binding.strength = 1.0;
				break;
				
				case GAMEPAD:
					if (binding.code == ZL_BUT)
						binding.strength = gamepad.axes [GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
					else if (binding.code == ZR_BUT)
						binding.strength = gamepad.axes [GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];
					else if (gamepad.buttons [binding.code])
						binding.strength = 1.0;
				break;
				
				case CLICK:
					button = glfwGetMouseButton (window, binding.code);
					if (button == GLFW_PRESS) binding.strength = 1.0;
				break;
				
				case MOUSE_MOVE:
					if (binding.code == AXIS_UP)
						binding.strength = abs (MIN (mouseY, 0));
					else if (binding.code == AXIS_DOWN)
						binding.strength = MAX (0, mouseY);
					else if (binding.code == AXIS_LEFT)
						binding.strength = abs (MIN (mouseX, 0));
					else if (binding.code == AXIS_RIGHT)
						binding.strength = MAX (mouseX, 0);
				break;
				
				case MOUSE_SCROLL:
					if (binding.code == AXIS_UP)
						binding.strength = abs (MIN (glfwScrollY, 0));
					else if (binding.code == AXIS_DOWN)
						binding.strength = MAX (0, glfwScrollY);
					else if (binding.code == AXIS_LEFT)
						binding.strength = abs (MIN (glfwScrollX, 0));
					else if (binding.code == AXIS_RIGHT)
						binding.strength = MAX (glfwScrollX, 0);
				break;
				
				case JOYSTICK:
					if (binding.code == AXIS_UP)
						binding.strength = abs (MIN (mouseY, 0));
					else if (binding.code == AXIS_DOWN)
						binding.strength = MAX (0, mouseY);
					else if (binding.code == AXIS_LEFT)
						binding.strength = abs (MIN (mouseX, 0));
					else if (binding.code == AXIS_RIGHT)
						binding.strength = MAX (mouseX, 0);
					
					if (binding.code == AXIS_NORTH)
						binding.strength = abs (MIN (gamepad.axes [GLFW_GAMEPAD_AXIS_LEFT_Y], 0));
					else if (binding.code == AXIS_SOUTH)
						binding.strength = MAX (0, gamepad.axes [GLFW_GAMEPAD_AXIS_LEFT_Y]);
					else if (binding.code == AXIS_WEST)
						binding.strength = abs (MIN (gamepad.axes [GLFW_GAMEPAD_AXIS_LEFT_X], 0));
					else if (binding.code == AXIS_EAST)
						binding.strength = MAX (gamepad.axes [GLFW_GAMEPAD_AXIS_LEFT_X], 0);
				break;
			}
			
			pressedActions [idx] = (binding.strength != 0.0);
			
			#ifdef DEBUG_INPUT
			
			if (buttonNow ((InputAction) idx))
				printf ("◌ %s\n", inputNames [idx]);
			
			#endif
			
			idx++;
		}
	}
	
	// release cursor
	void release ()
	{
		glfwSetInputMode (window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	
	// capture cursor
	void capture ()
	{
		glfwSetInputMode (window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	
	inline
	bool button (InputAction button)
	{
		return (pressedActions [button]);
	}
	
	inline
	bool buttonNow (InputAction button)
	{
		return (pressedActions [button] &&	!heldActions [button]);
	}
	
	inline
	void axisL (float* _x, float* _y)
	{
		*_x = mapping [NAV_RIGHT].strength - mapping[NAV_LEFT].strength;
		*_y = mapping [NAV_UP].strength - mapping [NAV_DOWN].strength;
	}
	
	inline
	void axisR (float* _x, float* _y)
	{
		*_x = mapping [POINT_RIGHT].strength - mapping[POINT_LEFT].strength;
		*_y = mapping [POINT_UP].strength - mapping [POINT_DOWN].strength;
	}
	
}
input;

// only way to access scroll input is through a callback
void zInputScrollCallback (GLFWwindow* window, double x, double y)
{
	input.glfwScrollX = x;
	input.glfwScrollY = y;
}