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

// No null ptrs allowed
    if (NULL == src || NULL == dest)
	return ERROR;

// Make sure we weren't asked to go outside our edges
    if (srcStartCol < 0 || (srcStartCol + srcLenCol) > srcSizeCol)
	return ERROR;
    if (destStartCol < 0 || (destStartCol + srcLenCol) > destSizeCol)
	return ERROR;
    if (srcStartRow < 0 || (srcStartRow + srcLenRow) > srcSizeRow)
	return ERROR;
    if (destStartRow < 0 || (destStartRow + srcLenRow) > destSizeRow)
	return ERROR;

//Check for overlap iff we're in the same array
    if (src == dest) {
	unsigned int                  flags = 0;

	// src starts to the right of dest
	flags |= srcStartCol >= (destStartCol + srcLenCol) ? 1 << 0 : 0;
	// dest starts to the right of src
	flags |= destStartCol >= (srcStartCol + srcLenCol) ? 1 << 1 : 0;
	// src starts below dest        
	flags |= srcStartRow >= (destStartRow + srcLenRow) ? 1 << 2 : 0;
	// dest starts below src        
	flags |= destStartRow >= (srcStartRow + srcLenRow) ? 1 << 3 : 0;

	// if any of the above are true, we don't intersect.  If all are false, we must.
	if (0 == flags)
	    return ERROR;
    }
// Do the flip
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
