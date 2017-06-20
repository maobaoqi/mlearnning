#include <fftw3.h>


int main(void)
{
	int n = 100;
	fftw_complex *in;
	fftw_complex *out;

	// allocate memory
	in = (fftw_complex*)fftw_malloc(n*sizeof(fftw_complex));
	out = (fftw_complex*)fftw_malloc(n * sizeof(fftw_complex));

	in[0][0] = 10;
	in[0][1] = 20;

	// creat a plan
	fftw_plan p;
	p = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

	// execute
	fftw_execute(p);

	fftw_destroy_plan(p);
	fftw_free(in);
	fftw_free(out);

	return 0;
}