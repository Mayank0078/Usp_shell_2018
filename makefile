all:prog

prog:shell.o
	gcc shell.c -o shell -g 
