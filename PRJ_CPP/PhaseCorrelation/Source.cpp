#include <fftw3.h>
#include <math.h>

#define BYTE unsigned int


void PhaseCorrelation2D(const BYTE *signal, 
	const BYTE *pattern, 
	int &heightShift, 
	int &widthShift)
{
	int rows = 512;
	int cols = 512;
	fftw_complex *ref_img = (fftw_complex*)fftw_malloc(rows * cols * sizeof(fftw_complex));
	fftw_complex *src_img = (fftw_complex*)fftw_malloc(rows * cols * sizeof(fftw_complex));

	// Plan
	fftw_plan plan_ref_forward = fftw_plan_dft_2d(rows, cols, ref_img, ref_img, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_plan plan_src_forward = fftw_plan_dft_2d(rows, cols, src_img, src_img, FFTW_FORWARD, FFTW_ESTIMATE);

	// Assign data
	for (int i = 0; i < rows * cols; i++)
	{
		src_img[i][0] = signal[i];
		src_img[i][1] = 0.0f;
	}

	for (int j = 0; j < rows * cols; j++)
	{
		ref_img[j][0] = pattern[j];
		ref_img[j][1] = 0.0f;
	}

	// Execute plan
	fftw_execute(plan_ref_forward);
	fftw_execute(plan_src_forward);

	// Calculate corss-correlation
	fftw_complex* cc_img = (fftw_complex*)fftw_malloc(rows * cols * sizeof(fftw_complex));
	double temp;
	for (int i = 0; i < rows * cols; i++)
	{
		cc_img[i][0] = (ref_img[i][0] * src_img[i][0]) - (src_img[i][1] * (-1.0f * ref_img[i][1]));
		cc_img[i][1] = (src_img[i][0] * (-1.0f * ref_img[i][1])) + (src_img[i][1] * ref_img[i][0]);

		temp = sqrt(cc_img[i][0] * cc_img[i][0] + cc_img[i][1] * cc_img[i][1]) + 0.001;

		cc_img[i][0] /= temp;
		cc_img[i][1] /= temp;
	}

	// ifft
	fftw_plan plan_cc_backfward = fftw_plan_dft_2d(rows, cols, cc_img, cc_img, FFTW_BACKWARD, FFTW_ESTIMATE);
	fftw_execute(plan_cc_backfward);

	fftw_destroy_plan(plan_ref_forward);
	fftw_destroy_plan(plan_src_forward);
	fftw_destroy_plan(plan_cc_backfward);
	fftw_free(src_img);
	fftw_free(ref_img);

	// Find peak
	double *cc = new double[rows * cols];
	for (int i = 0; i < rows * cols; i++)
	{
		cc[i] = cc_img[i][0];
	}

	int max_loc = 0;
	double max_value = 0.0;
	for (int i = 0; i < rows * cols; i++)
	{
		if (max_value < cc[i])
		{
			max_value = cc[i];
			max_loc = i;
		}
	}

	heightShift = floor(((int)max_loc) / cols);
	widthShift = (int)max_loc - cols * heightShift;

	if (heightShift > 0.5*rows)
	{
		heightShift = heightShift - rows;
	}
	if (widthShift > 0.5*cols)
	{
		widthShift = widthShift - cols;
	}

	delete[] cc;
	fftw_free(cc_img);
}