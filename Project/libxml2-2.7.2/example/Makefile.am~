noinst_PROGRAMS	=  exampleparse

INCLUDES = -I$(top_builddir)/include -I$(top_srcdir)/include -I@srcdir@/include @THREAD_CFLAGS@ @Z_CFLAGS@
DEPS = $(top_builddir)/libxml2.la
LDADDS = @STATIC_BINARIES@ $(top_builddir)/libxml2.la @THREAD_LIBS@ @Z_LIBS@ $(ICONV_LIBS) -lm @WIN32_EXTRA_LIBADD@

exampleparse_SOURCES=exampleparse.c
exampleparse_LDFLAGS=
exampleparse_DEPENDENCIES= $(DEPS)
exampleparse_LDADD= @RDL_LIBS@ $(LDADDS)
