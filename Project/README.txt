README

How to compile and run program:
	cd into fuse-2.6.3/example folder
	chmod 755 runscript
	./runscript
	./FS /tmp/fuse –o nonempty

 
In order to test other xml files:
	Edit bash script and replace 'example2.xml' with other name of xml file. File must be saved in libxml2-2.7.2/example folder.

How to test program:
	cd /tmp/fuse
	use test functions (ls, cd, cat, grep)

 

Troubleshooting:
	If there is a segmentation fault that returns a “Transport endpoint not connected” or a “software caused connection abort” error:
		cd
		fusermount –u /tmp/fuse
