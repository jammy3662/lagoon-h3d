#pragma once

typedef struct Bundle
{
	
}
Bundle;

Bundle* bundle = (Bundle*) 0;

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