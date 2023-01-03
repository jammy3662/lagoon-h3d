#pragma once

#define let auto

float waverage(float from, float to, float weight)
{
	return (from * (1-weight)) + (to * (weight));
}