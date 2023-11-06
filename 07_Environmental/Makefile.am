AM_CFLAGS = -std=c99 -lrhash

bin_PROGRAMS=rhasher
rhasher_SOURCES=rhasher.c

distclean:
	git clean -df

tests:
	echo -n "md5 \"linux-for-applications-2023" | ./rhasher | cut -sd'>' -f3 | cut -sd' ' -f2 | grep -vE '^$$' >rhasher.out
	echo -n "linux-for-applications-2023" | md5sum | cut -d' ' -f1 >md5.out
	cmp rhasher.out md5.out
	echo -n "sha1 \"linux-for-applications-2023" | ./rhasher | cut -sd'>' -f3 | cut -sd' ' -f2 | grep -vE '^$$' >rhasher.out
	echo -n "linux-for-applications-2023" | sha1sum | cut -d' ' -f1 >sha1.out
	cmp rhasher.out sha1.out

