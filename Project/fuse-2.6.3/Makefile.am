## Process this file with automake to produce Makefile.in

SUBDIRS = @subdirs@ @subdirs2@

EXTRA_DIST =			\
	fuse.pc.in		\
	README*			\
	Filesystems		\
	FAQ			\
	doc/how-fuse-works	\
	doc/kernel.txt

pkgconfigdir = @pkgconfigdir@
pkgconfig_DATA = fuse.pc

$(pkgconfig_DATA): config.status
