#include <ippi.h>
#include <ippcore.h>

#include <math.h>
#include <iostream>


float m_preGammaVal = 0.0f;

float SetGammaValue(float gammaVal)
{

	gammaVal *= 2.0f;
	gammaVal = (float)pow(gammaVal, 6);

	if (gammaVal < 1.0 / (1 << 6))
	{
		gammaVal = 1.0 / (1 << 6);
	}

	
	float m_gammaVal = gammaVal;

	if (m_preGammaVal != m_gammaVal)
	{
		//BuildTable(1.0f / m_gammaVal);
		m_preGammaVal = m_gammaVal;
	}

	return m_gammaVal;
}


int main(void)
{
	float gammaValue;
	gammaValue = SetGammaValue(0.2f);


	Ipp32f *src = new Ipp32f[4 * 4];
	IppiSize srcSize = { 4, 4 };
	ippiSet_32f_C1R(0.0f, src, 4 * sizeof(Ipp32f), { 4, 4 });
	src[0] = 2.0f;
	src[1] = 4.0f;
	src[2] = 8.0f;

	ippiExp_32f_C1R(src, 4 * sizeof(Ipp32f), src, 4 * sizeof(Ipp32f), srcSize);


	for (int i = 0; i < 4 * 4; i++)
	{
		std::cout << src[i] << " ";
		if ((i + 1) % 4 == 0)
		{
			std::cout << std::endl;
		}
	}

	// lut
	IppStatus status;
	Ipp32f pSrc[8 * 8];
	int srcStep = 8 * sizeof(Ipp32f);
	IppiSize roiSize = { 8, 8 };

	Ipp32f pDst[8 * 8];
	int dstStep = 8 * sizeof(Ipp32f);
	//Ipp32f pLevels[5] = { 0.0, 0.128, 0.256, 0.512, 1.0 };
	Ipp32f *pLevels = new Ipp32f[5];
	pLevels[0] = 0.0f;
	pLevels[1] = 1.0f;
	pLevels[2] = 4.0f;
	pLevels[3] = 10.0f;
	pLevels[4] = 16.0f;
	const Ipp32f *ppLevels[1] = { pLevels };
	//Ipp32f pValues[5] = { 0.2, 0.4, 0.6, 0.8, 1.0 };
	Ipp32f *pValues = new Ipp32f[5];
	pValues[0] = 1.0f;
	pValues[1] = 2.0f;
	pValues[2] = 8.0f;
	pValues[3] = 20.0f;
	pValues[4] = 32.0f;
	const Ipp32f *ppValues[1] = { pValues };
	int nLevels[1] = { 5 };
	int specSize;
	IppiLUT_Spec* pSpec;

	//status = ippiImageJaehne_32f_C1R(pSrc, srcStep, roiSize);
	ippiSet_32f_C1R(10.0f, pSrc, 8 * sizeof(Ipp32f), { 8, 8 });
	pSrc[0] = 4.0;
	
	ippiLUT_GetSize(ippLinear, ipp32f, ippC1, roiSize, nLevels, &specSize);

	pSpec = (IppiLUT_Spec*)ippMalloc(specSize);
	ippiLUT_Init_32f(ippLinear, ippC1, roiSize, ppValues, ppLevels, nLevels, pSpec);

	delete[] pLevels;
	delete[] pValues;

	status = ippiLUT_32f_C1R(pSrc, srcStep, pDst, dstStep, roiSize, pSpec);

	for (int i = 0; i < 8 * 8; i++)
	{
		std::cout << pDst[i] << " ";
		if ((i + 1) % 8 == 0)
		{
			std::cout << std::endl;
		}
	}


	ippFree(pSpec);

	return 0;
}