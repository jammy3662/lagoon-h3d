#pragma once

typedef unsigned int uint;

#define CLAMP(LO, X, HI) (MIN(MAX(LO, X), HI))
#define MIN(A, B) ((A < B) ? A : B)
#define MAX(A, B) ((A > B) ? A : B)

#define let constexpr static

struct float2 { float x, y; };
struct float3 { float x, y, z; };
struct float4 { float x, y, z, w; };

struct int2
{
	int x, y;
	operator float2 ()
	{ return {(float)x, (float)y}; }
};
struct int3
{
	int x, y, z;
	operator float3 ()
	{ return {(float)x, (float)y, (float)z}; }
};
struct int4
{
	int x, y, z, w;
	operator float4 ()
	{ return {(float)x, (float)y, (float)z, (float)w}; }
};

struct Color
{
	unsigned char r, g, b, a;
};