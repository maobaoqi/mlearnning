#pragma once

#include <fftw3.h>
#include <math.h>

#define BYTE unsigned char

void PhaseCorrelation2D(const BYTE *signal,
	const BYTE *pattern,
	int &heightShift,
	int &widthShift);