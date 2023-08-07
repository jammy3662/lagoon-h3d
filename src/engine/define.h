#pragma once

typedef unsigned int uint;

#define CLAMP(LO, X, HI) (MIN(MAX(LO, X), HI))
#define MIN(A, B) ((A < B) ? A : B)
#define MAX(A, B) ((A > B) ? A : B)

#define let constexpr static

typedef struct { float x, y; } float2;
typedef struct { float x, y, z; } float3;
typedef struct { float x, y, z, w; } float4;

typedef struct { int x, y; } int2;
typedef struct { int x, y, z; } int3;
typedef struct { int x, y, z, w; } int4;

/*
// buffer must be at least 3 bytes larger than output
unsigned long b64 (unsigned char* dst, unsigned char* src, long ln)
{
	char* fmt = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	unsigned long o = 0;
	for (; ln > 0; ln -= 4)
	{
		unsigned int v = 0;
		for (int i = 0; i < 4; i++)
		{
			int c = 0;
			if (ln > 3 || i < ln)
			{
				for (;fmt [c] != src [i]; c++) if (!fmt [c]) goto r;
				o++;
			}
			v = (v << 6) | c;
		}

		v = htonl (v) >> 8;
		*((int*) dst) = v;
		src += 4;
		dst += 3;
	}
r:
	return ((o*3) >> 2);
}
*/