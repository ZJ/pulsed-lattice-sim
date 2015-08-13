if HAVE_CHECK
# Don't force check, because only for testing

if !MINGWHOST
# Don't force check, because I don't know how to get it through mingw
TESTS = check_lens_setup.test

noinst_PROGRAMS = $(TESTS)
check_PROGRAMS = $(TESTS)

check_lens_setup_test_SOURCES = check_lens_setup.c
check_lens_setup_test_CFLAGS = @CHECK_CFLAGS@
check_lens_setup_test_LDADD = $(top_builddir)/src/.libs/liblens_setup.la @CHECK_LIBS@
endif # !MINGWHOST 
else

check-local:
	@echo "********************************************"
	@echo "Check not installed, so we cannot run tests!"
	@echo "********************************************"
	@false

endif # HAVE_CHECK
