all: md5 main

main: src/main.c
	mpicc -obin/md5_hack -Wall src/main.c src/md5/md5.c -lm

md5: src/md5/main.md5.c
	gcc -obin/md5 -Wall src/md5/main.md5.c src/md5/md5.c
