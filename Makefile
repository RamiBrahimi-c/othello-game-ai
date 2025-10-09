all:
	gcc -o main main.c -lraylib -lm -ldl -lpthread -lGL -lrt -lX11 
