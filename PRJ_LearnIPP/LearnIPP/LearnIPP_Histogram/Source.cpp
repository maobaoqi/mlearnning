#include <ippi.h>
#include <ipps.h>
#include <ippversion.h>
#include <ippcore.h>
#include <ippdefs.h>
#include <ippdc.h>
#include <ippversion.h>

void HistogramExample()
{
	const int HEIGHT = 8;
	const int WIDTH = 8;
	Ipp8u pImg[WIDTH * HEIGHT];
	IppiSize roi = { WIDTH, HEIGHT };
	int i;
	IppStatus status;
	{
		// fill image with random values in [0..255] range with uniform distribution.
		IppsRandUniState_8u* pRndObj;
		int sizeRndObj;

		// get spec size
		ippsRandUniformGetSize_8u(&sizeRndObj);
		pRndObj = (IppsRandUniState_8u*)ippsMalloc_8u(sizeRndObj);
		// initialize rnd spec
		ippsRandUniformInit_8u(pRndObj, 0, 255, 0);

		// fill image
		for (i = 0; i < HEIGHT; i++) {
			status = ippsRandUniform_8u(pImg + i*WIDTH, WIDTH, pRndObj);
		}

		ippsFree(pRndObj);
	}

	//printf_8u_2D
}