all:
	gcc -pthread  -std=gnu99 -Wpedantic dine.c -o dine
	gcc  -std=gnu99 -Wpedantic holes.c  -o holes