#ifndef FFT_H
#define FFT_H

#include <complex>
#include "vector.h"

namespace PondsMath{

class FFT{
public:

	FFT(unsigned int col_pow, unsigned int row_pow, unsigned int n);
	~FFT();
	void fft(std::complex<float> *data, unsigned int size_power, int sign = 1, unsigned int stride = 1);
	void fft2d(std::complex<float> *data, unsigned int row_pow, unsigned int col_pow, int sign = 1);
private:
	unsigned int reflect(unsigned int value, unsigned int limit);
	void reorder(std::complex<float> *data, unsigned int size_power, unsigned int stride = 1);
	std::complex<float>* powers;
	unsigned int row;
	unsigned int cols;
	unsigned int N;
	std::complex<float>* ptr;
};


}

#endif
