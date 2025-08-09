all: compila

compila:
	gcc -g -o trab3 *.c -lm

valgrind:
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all -s ./trab3

clean:
	rm -rf trab3