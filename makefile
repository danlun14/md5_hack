all: md5 main

main: src/main.c
	gcc -omd5_hack -Wall src/main.c src/md5/md5.c -lm

md5: src/md5/md5_main.c
	gcc -omd5 -Wall src/md5/md5_main.c src/md5/md5.c
