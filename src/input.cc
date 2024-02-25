#include "input.h"

#include <stdio.h>
#include <math.h>

#define MIN(A, B) ((A < B) ? A : B)
#define MAX(A, B) ((A > B) ? A : B)

const char* inputNames [] =
{
	"accept", "cancel", "info", "special",
	"menu", "pick", "alt", "shift",
	"more", "less", "prev", "next",
	"nav up", "nav down", "nav left", "nav right",
	"point up", "point down", "point left", "point right",
};

const Mapping mouseAndKeyboard =
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

const Mapping gamepadAndJoystick =
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

char debugInput = 0;

int gamepadIdx;
GLFWgamepadstate gamepad;

// relative movement
double mouseX, mouseY;
double lastMouseX, lastMouseY;

double glfwScrollX, glfwScrollY;

Mapping mapping = mouseAndKeyboard;

std::array <bool, INPUT_ACTION_CT> pressedActions;
std::array <bool, INPUT_ACTION_CT> heldActions;

// only way to access scroll input is through a callback
void inputScrollCallback (GLFWwindow* window, double x, double y)
{
	glfwScrollX = x;
	glfwScrollY = y;
}

void initInput ()
{
	// TODO: load saved controls from file
	
	glfwSetScrollCallback (window, inputScrollCallback);
	
	if (glfwRawMouseMotionSupported ())
	{
		glfwSetInputMode (window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
	
	//glfwSetInputMode (window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode (window, GLFW_STICKY_KEYS, GLFW_TRUE);
}
	
void refreshInput ()
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
	for (Binding& binding : mapping)
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
		
		if (debugInput && pressedNow ((InputAction) idx))
			printf ("◌ %s\n", inputNames [idx]);
		
		idx++;
	}
}

void releaseCursor ()
{
	glfwSetInputMode (window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void captureCursor ()
{
	glfwSetInputMode (window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

char pressed (InputAction button)
{
	return pressedActions [button];
}

char pressedNow (InputAction button)
{
	return pressedActions [button] && !heldActions [button];
}
	
float2 axisL ()
{
	return
	{
		mapping [NAV_RIGHT].strength - mapping[NAV_LEFT].strength,
		mapping [NAV_UP].strength - mapping [NAV_DOWN].strength
	};
}

float2 axisR ()
{
	return
	{
		mapping [POINT_RIGHT].strength - mapping[POINT_LEFT].strength,
		mapping [POINT_UP].strength - mapping [POINT_DOWN].strength
	};
}
