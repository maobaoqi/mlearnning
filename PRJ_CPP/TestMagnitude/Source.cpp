#include <ipp.h>
#include <ippi.h>

float MyPOC(const Ipp32f * const pfImage1, const Ipp32f * const pfImage2, const int iSize, const int iOrder, float &   rfDX, float &   rfDY)
{
	float fPeakVal(-1.f);

	IppiFFTSpec_R_32f * pzSpecFFT(nullptr);
	if (ippStsNoErr == ippiFFTInitAlloc_R_32f(&pzSpecFFT, iOrder, iOrder, IPP_FFT_NODIV_BY_ANY, ippAlgHintAccurate))
	{
		//  一時画像の確保
		int iStep;
		int iStepUnpack;
		const IppiSize zImageSize = { iSize, iSize };
		Ipp32f  * pfFFT1(ippiMalloc_32f_C1(iSize, iSize, &iStep));        //  pImage1のFFTの結果
		Ipp32f  * pfFFT2(ippiMalloc_32f_C1(iSize, iSize, &iStep));        //  pImage2のFFTの結果
		Ipp32f  * pfMulC(ippiMalloc_32f_C1(iSize, iSize, &iStep));        //  FFT1とFFT2の共役複素の積
		Ipp32fc * pfcMulC(ippiMalloc_32fc_C1(iSize, iSize, &iStepUnpack));  //  RCPack2D形式でないpfMulC   
		Ipp32f  * pfAmp(ippiMalloc_32f_C1(iSize, iSize, &iStep));        //  pfMulCの振幅
		Ipp32f  * pfPOI(ippiMalloc_32f_C1(iSize, iSize, &iStep));        //  pfMulCの位相
		Ipp32f  * pfDst(ippiMalloc_32f_C1(iSize, iSize, &iStep));        //  pfPOIの逆FFTの結果
		if (pfFFT1 && pfFFT2 && pfMulC && pfcMulC && pfAmp && pfPOI && pfDst)
		{
			//  image1のFFT
			if (ippStsNoErr == ippiFFTFwd_RToPack_32f_C1R(pfImg1, iStep, pfFFT1, iStep, pzSpecFFT, 0))
			{
				//  image2のFFT
				if (ippStsNoErr == ippiFFTFwd_RToPack_32f_C1R(pfImg2, iStep, pfFFT2, iStep, pzSpecFFT, 0))
				{
					//  FFT1とFFT2の共役複素の積
					if (ippStsNoErr == ippiMulPackConj_32f_C1R(pfFFT1, iStep, pfFFT2, iStep, pfMulC, iStep, zImageSize))
					{
						//  RCPack2D形式から実数形式に変換
						if (ippStsNoErr == ippiPackToCplxExtend_32f32fc_C1R(pfMulC, zImageSize, iStep, pfcMulC, iStepUnpack))
						{
							//  振幅に変換   
							if (ippStsNoErr == ippiMagnitudePack_32f_C1R(pfMulC, iStep, pfAmp, iStep, zImageSize))
							{
								//  正規化して位相を求める
								const int iArea(iSize * iSize);
								Ipp32fc * pfcPtrMulC(pfcMulC);
								Ipp32f  * pfPtrAmp(pfAmp);
								for (int i = 0; i < iArea; ++i)
								{
									const Ipp32f fAmp(*pfPtrAmp);
									if (0.f < fAmp)
									{
										pfcPtrMulC->re /= fAmp;
										pfcPtrMulC->im /= fAmp;
									}
									++pfcPtrMulC;
									++pfPtrAmp;
								}
								//  RCPack2D形式变换
								if (ippStsNoErr == ippiCplxExtendToPack_32fc32f_C1R(pfcMulC, iStepUnpack, zImageSize, pfPOI, iStep))
								{
									//  逆FFTを実行
									if (ippStsNoErr == ippiFFTInv_PackToR_32f_C1R(pfPOI, iStep, pfDst, iStep, pzSpecFFT, 0))
									{
										//  ピークを探す
										fPeakVal = (*pfDst);
										rfDX = rfDY = 0.f;
										const Ipp32f * pfPtrDst(pfDst + 1);
										for (int i = 1; i < iArea; ++i)
										{
											if ((*pfPtrDst) > fPeakVal)
											{
												fPeakVal = (*pfPtrDst);
												rfDX = i % iSize;
												rfDY = i / iSize;
											}
											++pfPtrDst;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		//  一時画像の開放
		if (pfDst) { ippiFree(pfDst); }
		if (pfPOI) { ippiFree(pfPOI); }
		if (pfAmp) { ippiFree(pfAmp); }
		if (pfcMulC) { ippiFree(pfcMulC); }
		if (pfMulC) { ippiFree(pfMulC); }
		if (pfFFT2) { ippiFree(pfFFT2); }
		if (pfFFT1) { ippiFree(pfFFT1); }
	}
	if (pzSpecFFT) { ippiFFTFree_R_32f(pzSpecFFT); }

	return(fPeakVal);
}