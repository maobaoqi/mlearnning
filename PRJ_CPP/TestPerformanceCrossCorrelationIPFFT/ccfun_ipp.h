#pragma once
#include <ippi.h>
#include <ipp.h>

#include <ctime>

void PhaseCorrelation(Ipp8u* ref, Ipp8u* src, IppiSize roi, int &wshift, int &hshift);