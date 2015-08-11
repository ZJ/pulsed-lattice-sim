#include <stdlib.h>
#include <check.h>
#include <math.h>

#include "../src/lens_setup.h"

const fftw_complex  clearedVal = -1.0 + I * 0.0;

fftw_complex       *toFlip;
fftw_complex       *flipped;

void checkClearArray(
) {
    if (NULL != toFlip)
	free(toFlip);
    toFlip = NULL;
    if (NULL != flipped)
	free(flipped);
    flipped = NULL;
}

void init_indexes(
    fftw_complex * toInit,
    int nCols,
    int nRows
) {
    int                 i,
                        j;

    for (i = 0; i < nRows; i++) {
	for (j = 0; j < nCols; j++) {
	    toInit[i * nCols + j] = i + I * j;
	}
    }
}

void init_cleared(
    fftw_complex * toInit,
    int nCols,
    int nRows
) {
    int                 i,
                        j;
    int                 total = nCols * nRows;

    for (i = 0; i < total; i++) {
	toInit[i] = clearedVal;
    }
}

void setup_blanks(
) {
    toFlip = fftw_alloc_complex(20 * 20);
    flipped = fftw_alloc_complex(20 * 20);
    init_cleared(toFlip, 20, 20);
    init_cleared(flipped, 20, 20);
}

#define NUM_FLIP_BOUNDS 16
START_TEST(flip_bounds) {
    const char         *testDetails[NUM_FLIP_BOUNDS] = {
	"Region starts past last source column.",
	"Region ends past last source column.",
	"Region starts past last source row.",
	"Region end past last source row.",
	"Negative start source column.",
	"Negative start source row.",
	"Region starts past last destination column.",
	"Region ends past last destination column.",
	"Region starts past last destination row.",
	"Region end past last destination row.",
	"Negative start destination column.",
	"Negative start destination row.",
	"Source-Destination top-bottom intersection.",
	"Source-Destination left-right intersection.",
	"Source-Destination bottom-top intersection.",
	"Source-Destination right-left intersection."
    };
    fftw_complex       *srcPtr = toFlip;
    fftw_complex       *destPtr = flipped;
    int                 srcCol = 0;
    int                 srcRow = 0;
    int                 srcColLen = 10;
    int                 srcRowLen = 10;
    int                 destRow = 0;
    int                 destCol = 0;
    const int           dimLen = 20;
    int                 runNum = _i % NUM_FLIP_BOUNDS;

    switch (runNum) {
    case 0:
	srcCol = dimLen;
	break;
    case 1:
	srcCol = dimLen - srcColLen + 1;
	break;
    case 2:
	srcRow = dimLen;
	break;
    case 3:
	srcRow = dimLen - srcRowLen + 1;
	break;
    case 4:
	srcCol = -1;
	break;
    case 5:
	srcRow = -1;
	break;
    case 6:
	destCol = dimLen;
	break;
    case 7:
	destCol = dimLen - srcColLen + 1;
	break;
    case 8:
	destRow = dimLen;
	break;
    case 9:
	destRow = dimLen - srcRowLen + 1;
	break;
    case 10:
	destCol = -1;
	break;
    case 11:
	destRow = -1;
	break;
    case 12:
	destPtr = srcPtr;
	srcRow = srcRowLen - 1;
	break;
    case 13:
	destPtr = srcPtr;
	destCol = srcColLen - 1;
	break;
    case 14:
	destPtr = srcPtr;
	destRow = srcRowLen - 1;
	break;
    case 15:
	destPtr = srcPtr;
	srcCol = srcColLen - 1;
	break;
    default:
	ck_abort_msg("Got to invalid test case in flip_(bounds) (runNum = %d)", runNum);
    }

    ck_assert_msg(ERROR ==
		  vert_flip_region(srcPtr, dimLen, dimLen, srcCol, srcRow, srcColLen, srcColLen,
				   destPtr, dimLen, dimLen, destCol, destRow),
		  "vert_flip_region() failed to flag bounds violation %s.", testDetails[runNum]);
}

END_TEST
#define NUM_FLIP_BOUNDS_SNUGGLES 5
START_TEST(
    flip_bounds_snuggle
) {
    const char         *testDetails[NUM_FLIP_BOUNDS_SNUGGLES] = {
	"Source above    destination",
	"Source left of  destination",
	"Source below    destination",
	"Source right of destination",
	"Source in same configuration but different array from destination"
    };
    fftw_complex       *destPtr = toFlip;
    int                 runNum = _i % NUM_FLIP_BOUNDS_SNUGGLES;
    int                 srcCol = 0;
    int                 srcRow = 0;
    int                 destCol = 0;
    int                 destRow = 0;

    switch (runNum) {
    case 0:
	destRow = 10;
	break;
    case 1:
	destCol = 10;
	break;
    case 2:
	srcRow = 10;
	break;
    case 3:
	srcCol = 10;
	break;
    case 4:
	destPtr = flipped;
	break;
    default:
	ck_abort_msg("Invalid test reached in flip_bounds_snuggle() [runNum = %d]", runNum);
    }


    ck_assert_msg(OK ==
		  vert_flip_region(toFlip, 20, 20, srcCol, srcRow, 10, 10, destPtr, 20, 20,
				   destCol, destRow),
		  "vert_flip_region() erroneously failed with %s when snuggled.",
		  testDetails[runNum]);

}

END_TEST
#define NUM_FLIP_NULL_PTR 2
START_TEST(
    flip_null_ptr
) {
    const char         *testDetails[NUM_FLIP_NULL_PTR] =
	{ "Source NULL ptr.", "Destination NULL ptr." };

    fftw_complex       *srcPtr = toFlip;
    fftw_complex       *destPtr = flipped;
    int                 runNum = _i % NUM_FLIP_NULL_PTR;

    switch (runNum) {
    case 0:
	srcPtr = NULL;
	break;
    case 1:
	destPtr = NULL;
	break;
    default:
	ck_abort_msg("Reached undefined test case (%d).", runNum);
    }


    ck_assert_msg(ERROR ==
		  vert_flip_region(srcPtr, 20, 20, 1, 1, 5, 5, destPtr, 20, 20, 10, 10),
		  "vert_flip_region() did not fail with %s", testDetails[runNum]);
}

END_TEST START_TEST(
    flip_region
) {
    int                 i,
                        j;
    int                 colsize = 20;
    int                 rowsize = 20;
    int                 srccolstart = 2;
    int                 srccollen = 10;
    int                 srcrowstart = 3;
    int                 srcrowlen = 7;
    int                 destcolst = 5;
    int                 destrowst = 12;
    int                 startCheck,
                        stopCheck;

    toFlip = fftw_alloc_complex(colsize * rowsize);
    flipped = fftw_alloc_complex(colsize * rowsize);

    init_indexes(toFlip, colsize, rowsize);
    init_cleared(flipped, colsize, rowsize);

    ck_assert_msg(OK ==
		  vert_flip_region(toFlip, colsize, rowsize, srccolstart, srcrowstart,
				   srccollen, srcrowlen, flipped, colsize, rowsize, destcolst,
				   destrowst), "vert_flip_region() unexpectly failed");

// Checking we don't touch regions we shouldn't
    // Bottom area
    startCheck = (destrowst + srcrowlen) * colsize;
    stopCheck = rowsize * colsize;
    for (i = startCheck; i < stopCheck; i++) {
	ck_assert_msg(-1.0 + I * 0.0 == flipped[i],
		      "Altered below bottom boundary, found %d +i%d at row %d, col %d",
		      (int) creal(flipped[i]), (int) cimag(flipped[i]), i / colsize, i % colsize);
    }
    // Top area
    startCheck = 0;
    stopCheck = destrowst * colsize;
    for (i = startCheck; i < stopCheck; i++) {
	ck_assert_msg(-1.0 + I * 0.0 == flipped[i],
		      "Altered above top boundary, found %d +i%d at row %d, col %d",
		      (int) creal(flipped[i]), (int) cimag(flipped[i]), i / colsize, i % colsize);
    }
    // Sides
    startCheck = destrowst * colsize;
    stopCheck = (destrowst + srcrowlen) * colsize;
    for (i = startCheck; i < stopCheck; i += colsize) {
	int                 rightedge = destcolst + srccollen;

	// Left edge
	for (j = 0; j < destcolst; j++) {
	    ck_assert_msg(-1.0 + I * 0.0 == flipped[i + j],
			  "Altered left of left boundary, found %d +i%d at row %d, col %d",
			  (int) creal(flipped[i + j]), (int) cimag(flipped[i + j]), i / colsize, j);
	}
	// Right edge
	for (j = rightedge; j < rowsize; j++) {
	    ck_assert_msg(-1.0 + I * 0.0 == flipped[i + j],
			  "Altered right of right boundary, found %d +i%d at row %d, col %d",
			  (int) creal(flipped[i + j]), (int) cimag(flipped[i + j]), i / colsize, j);
	}
    }

// Check we DO take care of regions we should
    startCheck = destrowst + srcrowlen - 1;
    for (i = 0; i < srcrowlen; i++) {
	int                 srclinest = (srcrowstart + i) * colsize + srccolstart;
	int                 destlinest = (startCheck - i) * colsize + destcolst;

	for (j = 0; j < srccollen; j++) {
	    ck_assert_msg(toFlip[srclinest + j] == flipped[destlinest + j],
			  "Did not flip properly. Expected %d + i%d, found %d + i%d",
			  (int) creal(toFlip[srclinest + j]),
			  (int) cimag(toFlip[srclinest + j]),
			  (int) creal(flipped[destlinest + j]),
			  (int) cimag(flipped[destlinest + j]));
	}
    }
}

END_TEST START_TEST(
    flip_top
) {
    fftw_complex       *preflip;
    int                 i,
                        j;
    const int           halfPoint = _i / 2;
    const int           fullPoint = _i - 1;
    const int           count = _i;

    preflip = fftw_alloc_complex(count * count);
    toFlip = preflip;

    for (i = 0; i < count; i++) {
	for (j = 0; j < count; j++) {
	    preflip[i * count + j] = i + I * j;
	}
    }

    ck_assert_msg(OK == vert_flip_top(preflip, count, count), "vert_flip_top() did not return OK");

    for (i = 0; i < halfPoint; i++) {
	for (j = 0; j < count; j++) {
	    ck_assert_msg(i + I * j == preflip[i * count + j],
			  "Modified wrong part of the array. Expected %d + i%d, found %d + i%d",
			  i, j, (int) creal(preflip[i * count + j]),
			  (int) cimag(preflip[i * count + j]));
	}
    }
    if (0 != _i % 2) {
	i = halfPoint;
	for (j = 0; j < count; j++) {
	    ck_assert_msg(i + I * j == preflip[i * count + j],
			  "Center line of odd-length array modifed. Expected %d + i%d, found %d + i%d",
			  i, j, (int) creal(preflip[i * count + j]),
			  (int) cimag(preflip[i * count + j]));
	}
    }
    for (i = 0; i < halfPoint; i++) {
	for (j = 0; j < count; j++) {
	    ck_assert_msg(i + I * j == preflip[(fullPoint - i) * count + j],
			  "Array is unflipped. Expected %d + i%d, found %d + i%d", i, j,
			  (int) creal(preflip[(fullPoint - i) * count + j]),
			  (int) cimag(preflip[(fullPoint - i) * count + j]));
	}
    }


}

END_TEST START_TEST(
    flip_bot
) {
    fftw_complex       *preflip;
    int                 i,
                        j;
    const int           halfPoint = _i / 2;
    const int           fullPoint = _i - 1;
    const int           count = _i;

    preflip = fftw_alloc_complex(count * count);
    toFlip = preflip;

    for (i = 0; i < count; i++) {
	for (j = 0; j < count; j++) {
	    preflip[i * count + j] = i + I * j;
	}
    }

    ck_assert_msg(OK == vert_flip_bot(preflip, count, count), "vert_flip_bot() did not return OK");

    for (i = halfPoint; i < count; i++) {
	for (j = 0; j < count; j++) {
	    ck_assert_msg(i + I * j == preflip[i * count + j],
			  "Modified wrong part of the array. Expected %d + i%d, found %d + i%d",
			  i, j, (int) creal(preflip[i * count + j]),
			  (int) cimag(preflip[i * count + j]));
	}
    }
    for (i = 0; i < halfPoint; i++) {
	for (j = 0; j < count; j++) {
	    ck_assert_msg((fullPoint - i) + I * j == preflip[i * count + j],
			  "Array is unflipped. Expected %d + i%d, found %d + i%d",
			  fullPoint - i, j, (int) creal(preflip[i * count + j]),
			  (int) cimag(preflip[i * count + j]));
	}
    }


}

END_TEST Suite     *flip_suite(
    void
) {
    Suite              *s;
    TCase              *tc_core;

    s = suite_create("Flip Arrays");

    tc_core = tcase_create("flip_verify_function");
    tcase_add_checked_fixture(tc_core, checkClearArray, checkClearArray);
    tcase_add_test(tc_core, flip_region);
    tcase_add_loop_test(tc_core, flip_top, 10, 14);
    tcase_add_loop_test(tc_core, flip_bot, 10, 14);
    suite_add_tcase(s, tc_core);

    tc_core = tcase_create("flip_verify_bounds");
    tcase_add_unchecked_fixture(tc_core, setup_blanks, checkClearArray);
    tcase_add_loop_test(tc_core, flip_null_ptr, 0, NUM_FLIP_NULL_PTR);
    tcase_add_loop_test(tc_core, flip_bounds_snuggle, 0, NUM_FLIP_BOUNDS_SNUGGLES);
    tcase_add_loop_test(tc_core, flip_bounds, 0, NUM_FLIP_BOUNDS);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(
    void
) {
    int                 number_failed;
    Suite              *s;
    SRunner            *sr;

    s = flip_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
