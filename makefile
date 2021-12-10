all: md5 main serial_main

main: src/main.c
	mpicc -obin/md5_hack -Wall src/main.c src/md5/md5.c -lm

serial_main: src/main.serial.c
	mpicc -obin/md5_hacks -Wall src/main.serial.c src/md5/md5.c -lm

md5: src/md5/main.md5.c
	gcc -obin/md5 -Wall src/md5/main.md5.c src/md5/md5.c
