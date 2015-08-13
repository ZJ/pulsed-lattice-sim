lib_LTLIBRARIES = liblens_setup.la
liblens_setup_la_SOURCES = lens_setup.c lens_setup.h retCodes.h
if MINGWHOST
MINGWLDFLAGS = -static-libgcc -static-libstdc++ -no-undefined
else
MIGWLDFLAGS =
endif
liblens_setup_la_LDFLAGS = $(MINGWLDFLAGS)

bin_PROGRAMS = simpleTest
simpleTest_SOURCES = main.c
simpleTest_LDFLAGS = $(MINGWLDFLAGS)
simpleTest_LDADD = $(top_srcdir)/winlibs/$(host)/libfftw3-3.dll 
