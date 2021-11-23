if LIBTSTAT
    target = demo
else
    target =
endif
AM_CFLAGS = -I$(top_srcdir)/include

noinst_PROGRAMS = $(target)
bin_PROGRAMS = $(target)
demo_SOURCES = \
    pcapwrapper.c demo.c\
    pcapwrapper.h

if OSX
demo_LDADD = $(top_srcdir)/libtstat/.libs/libtstat.dylib
demo_LDFLAGS = -Wl,-rpath,$(top_srcdir)/libtstat/.libs
else
demo_LDADD = $(top_srcdir)/libtstat/.libs/libtstat.so
demo_LDFLAGS = -Wl,--rpath -Wl,$(top_srcdir)/libtstat/.libs
endif

EXTRA_DIST = tstat.conf configure.ac
