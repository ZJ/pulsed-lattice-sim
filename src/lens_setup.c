#include "lens_setup.h"

#include "retCodes.h"		       // Our internally defined return codes

#include <complex.h>		       // Make sure complex types are setup for fftw load
#include <fftw3.h>		       // Get fftw types if needed
#include <string.h>		       // Get memcpy()

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
    fftw_complex                 *srcLine = src + (srcStartRow * srcSizeCol) + srcStartCol,
	*destLine = dest + (destStartRow * destSizeCol) + destStartCol;
    size_t                        lineLen = sizeof (fftw_complex) * srcLenCol;
    int                           copyRow;

    for (copyRow = 0; copyRow < srcLenRow; copyRow++) {
	memcpy(destLine + ((srcLenRow - 1 - copyRow) * destSizeCol),
	       srcLine + (copyRow * srcSizeCol), lineLen);
    }

    return OK;
}

retCodes_type vert_flip_top(
    fftw_complex * toFlip,
    int sizeCol,
    int sizeRow
) {
    return vert_flip_region(toFlip, sizeCol, sizeRow, 0, 0, sizeCol, sizeRow / 2, toFlip,
			    sizeCol, sizeRow, 0, sizeRow / 2 + sizeRow % 2);
}

retCodes_type vert_flip_bot(
    fftw_complex * toFlip,
    int sizeCol,
    int sizeRow
) {
    return vert_flip_region(toFlip, sizeCol, sizeRow, 0, sizeRow / 2 + sizeRow % 2, sizeCol,
			    sizeRow / 2, toFlip, sizeCol, sizeRow, 0, 0);
}
