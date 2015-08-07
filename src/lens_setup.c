#include "lens_setup.h"

#include "retCodes.h"		       // Our internally defined return codes

#include <complex.h>		       // Make sure complex types are setup for fftw load
#include <fftw3.h>		       // Get fftw types if needed


retCodes_type vert_flip_region(
    fftw_complex * src,
    int srcSizeCol,
    int srcSizeRow,
    int srcStartCol,
    int srcStartRow,
    int srcLenCol,
    int srcLenRow,
    fftw_complex * dest,
    int destSizeCol,
    int destSizeRow,
    int destStartCol,
    int destStartRow
) {
    return OK;
}

retCodes_type vert_flip_top(
    fftw_complex * toFlip,
    int sizeCol,
    int sizeRow
) {
    return OK;
}

retCodes_type vert_flip_bot(
    fftw_complex * toFlip,
    int sizeCol,
    int sizeRow
) {
    return OK;
}
