#include <stdlib.h>
#include <check.h>

START_TEST(test_success_always) {
    ck_assert_int_eq(1, 1);
}

END_TEST Suite               *dummy_suite(
    void
) {
    Suite                        *s;
    TCase                        *tc_core;

    s = suite_create("Dummy");

    tc_core = tcase_create("Success");

    tcase_add_test(tc_core, test_success_always);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(
    void
) {
    int                           number_failed;
    Suite                        *s;
    SRunner                      *sr;

    s = dummy_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
