#include <complex.h>
#include <stdio.h>
#include "lib/fftw-3.3.4-dll64/fftw3.h"

#define N 4096

double phaseFunc(int x) {
	
}

int main(int argc, char ** argv) {
	fftw_complex * testPtr = NULL;
	int i = 0;
	
	i++;
	testPtr = fftw_alloc_complex(N*N);
	printf("Pointer:\t0x%p\n", testPtr);
	
	
}
