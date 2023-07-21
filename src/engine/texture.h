#pragma once

struct Texture
{
	unsigned int id;
};

// load texture from file at path
Texture loadTexture (const char* path);

void drawTexture (Texture texture, int x, int y, int wdt, int hgt);

void drawTexture (Texture texture, float x, float y, float z, int wdt, int hgt);

// texture faces camera
void drawBillboard (Texture texture, float x, float y, float z);

struct Frame
{
	unsigned int fbo;
};