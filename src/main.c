#include <complex.h>
#include <stdio.h>
#include <fftw3.h>

#include "retCodes.h"

#define N 4096

/*!	@brief Generates a real-valued Gaussian envelope
 *
 *	@details Overwrites the array at inputArray with a real-valued Gaussian envelope function
 *	centered within the array.
 *	
 *	If inputArray is a non-NULL pointer, it will attempt to write to that memory location.
 *	If it is NULL, will attempt to use fftw_alloc_complex() to allocate a new, empty array.
 *
 *	@param[inout]   inputArray	Pointer to the array to fill.  If NULL and non-zero sideLength, attempts to allocate the array itself.
 *	@param[in]      numRows		Number of rows in the array
 *  @param[in]		numCols		Number of columns in the array
 *	@param[in]		rowWaist	Scaled Gaussian waist along a row, 
 *	@param[in]		colWaist	Scaled Gaussian waist along a column
 *	@param[in]		amplitude	Amplitude of the Gaussian
 */
memRetCodes_type initGaussianEnvelope(
    fftw_complex ** inputArray,
    const unsigned int numRows,
    const unsigned int numCols,
    const double rowWaist,
    const double colWaist,
    const double amplitude
) {
    int                           i = 0;
    int                           j = 0;
    double                        iCenter;
    double                        jCenter;
    const double                  rowDenom = 2 * rowWaist * rowWaist;
    const double                  colDenom = 2 * colWaist * colWaist;
    unsigned int                  numCells = numRows * numCols;

    // Check for improperly formatted size info
    if (0 == numRows) {
	return ERR_MEM_NOSIZE;
    }
    // Handle NULL ptr
    if (NULL == *inputArray) {
	fftw_complex                 *tempPtr = NULL;

	tempPtr = fftw_alloc_complex(numCells);
	if (NULL == tempPtr) {
	    return ERR_MEM_ALLOC;
	}
	*inputArray = tempPtr;
    }
    // Do the Gaussian
    iCenter = ((double) numCols) / 2.0;
    jCenter = ((double) numRows) / 2.0;
    for (i = 0; i < numRows; i++) {
	double                        rowAmp =
	    amplitude * exp(-((i - iCenter) * (i - iCenter)) / rowDenom);
	int                           iOffset = i * numCols;

	for (j = 0; j < numCols; j++) {
	    *(*inputArray + iOffset + j) =
		rowAmp * exp(-((j - jCenter) * (j - jCenter)) / colDenom);
	}
    }

    return OK_MEM;
}

int main(
    int argc,
    char **argv
) {
    fftw_complex                 *testPtr = NULL;
    int                           i = 0;

    i++;
    testPtr = fftw_alloc_complex(N * N);
    printf("Pointer:\t0x%p\n", testPtr);
}
