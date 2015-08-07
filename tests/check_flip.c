#include <stdlib.h>
#include <check.h>
#include <math.h>

#include "../src/lens_setup.h"

const fftw_complex            clearedVal = -1.0 + I * 0.0;

fftw_complex                 *toFlip;
fftw_complex                 *flipped;

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
    int                           i,
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
    int                           i,
                                  j;
    int                           total = nCols * nRows;

    for (i = 0; i < total; i++) {
	toInit[i] = clearedVal;
    }
}

START_TEST(flip_region) {
    int                           i,
                                  j;
    int                           colsize = 20;
    int                           rowsize = 20;
    int                           srccolstart = 2;
    int                           srccollen = 10;
    int                           srcrowstart = 3;
    int                           srcrowlen = 7;
    int                           destcolst = 5;
    int                           destrowst = 12;
    int                           startCheck,
                                  stopCheck;

    toFlip = fftw_alloc_complex(colsize * rowsize);
    flipped = fftw_alloc_complex(colsize * rowsize);

    init_indexes(toFlip, colsize, rowsize);
    init_cleared(flipped, colsize, rowsize);

    ck_assert_msg(OK ==
		  vert_flip_region(toFlip, colsize, rowsize, srccolstart, srcrowstart, srccollen,
				   srcrowlen, flipped, colsize, rowsize, destcolst, destrowst),
		  "vert_flip_region() unexpectly failed");

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
	int                           rightedge = destcolst + srccollen;

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
	int                           srclinest = (srcrowstart + i) * colsize + srccolstart;
	int                           destlinest = (startCheck - i) * colsize + destcolst;

	for (j = 0; j < srccollen; j++) {
	    ck_assert_msg(toFlip[srclinest + j] == flipped[destlinest + j],
			  "Did not flip properly. Expected %d + i%d, found %d + i%d",
			  (int) creal(toFlip[srclinest + j]),
			  (int) cimag(toFlip[srclinest + j]), (int) creal(flipped[destlinest + j]),
			  (int) cimag(flipped[destlinest + j]));
	}
    }
}

END_TEST START_TEST(
    flip_top
) {
    fftw_complex                 *preflip;
    int                           i,
                                  j;
    const int                     halfPoint = _i / 2;
    const int                     fullPoint = _i - 1;
    const int                     count = _i;

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
    fftw_complex                 *preflip;
    int                           i,
                                  j;
    const int                     halfPoint = _i / 2;
    const int                     fullPoint = _i - 1;
    const int                     count = _i;

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

END_TEST Suite               *flip_suite(
    void
) {
    Suite                        *s;
    TCase                        *tc_core;

    s = suite_create("Flip Arrays");

    tc_core = tcase_create("test_flip");
    tcase_add_checked_fixture(tc_core, checkClearArray, checkClearArray);
    tcase_add_test(tc_core, flip_region);
    tcase_add_loop_test(tc_core, flip_top, 10, 14);
    tcase_add_loop_test(tc_core, flip_bot, 10, 14);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(
    void
) {
    int                           number_failed;
    Suite                        *s;
    SRunner                      *sr;

    s = flip_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
