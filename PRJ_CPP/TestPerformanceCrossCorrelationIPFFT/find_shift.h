#pragma once
#include <math.h>
#include <memory>

#include <ippi.h>
#include <ipp.h>

struct LocationPair
{
	float x;
	float y;
};

void FindShift(float* ccMap, LocationPair & shift, int width, int height, LocationPair roughShift);

