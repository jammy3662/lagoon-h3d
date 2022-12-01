#pragma once

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#define let auto

#define vec2 Vector2
#define vec3 Vector3
#define vec4 Vector4

// key binds
int knorth = KEY_W;
int ksouth = KEY_S;
int keast = KEY_D;
int kwest = KEY_A;

int kshift = KEY_LEFT_SHIFT;
int kctrl = KEY_LEFT_CONTROL;
int kjump = KEY_SPACE;
int ktab = KEY_TAB;

int kup = KEY_UP;
int kdown = KEY_DOWN;
int kright = KEY_RIGHT;
int kleft = KEY_LEFT;

int ktrigger = MOUSE_BUTTON_LEFT;
int kbumper = MOUSE_BUTTON_RIGHT;

int kexit = KEY_ESCAPE;

// frame (internal render target / backbuffer)
int fwidth = 1920;
int fheight = 1080;

// window (onscreen drawing)
int wwidth = 1280;
int wheight = 720;