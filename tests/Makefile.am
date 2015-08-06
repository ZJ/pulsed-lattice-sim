if HAVE_CHECK # Don't force check, because only for testing
TESTS = check_dummy

noinst_PROGRAMS = $(TESTS)
check_PROGRAMS = $(TESTS)

check_dummy_SOURCES = check_dummy.c
check_dummy_CFLAGS = @CHECK_CFLAGS@
check_dummy_LDADD = @CHECK_LIBS@

endif # HAVE_CHECK
