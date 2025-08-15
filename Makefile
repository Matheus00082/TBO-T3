all: compila

compila:
	gcc -g -o trab3 *.c -lm

valgrind:
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all -s ./trab3 ./exemplo

clean:
	rm -rf trab3

time:
	time ./trab3 ./exemplo

kb:
	# memória máxima usada pelo processo em kb
	/usr/bin/time -v ./trab3 ./exemplo

teste 1:
	./trab3 ./testes/1