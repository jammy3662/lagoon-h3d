#pragma once

#include "define.h"

struct Texture
{
	uint id;
};

// load texture from file at path
Texture loadTexture (const char* path);

void drawTexture (Texture texture, int x, int y, int width, int height);

void drawTexture (Texture texture, float x, float y, float z, int width, int height);

// texture faces camera
void drawBillboard (Texture texture, float x, float y, float z);
