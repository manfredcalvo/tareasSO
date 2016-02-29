


CC=gcc
CFLAGS=-I

rastreadormake: Tarea\#1.c syscallents.h syscalls.h
	$(CC) -o rastreador "Tarea#1.c" $(CFLAGS).
