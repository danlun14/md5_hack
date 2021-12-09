all: src/main.c
	gcc -omd5_hack -Wall src/main.c src/md5/md5.c
