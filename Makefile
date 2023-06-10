build: src/brainfuck.c
	gcc -O3 -Wall -Werror -Wextra -Wpedantic src/brainfuck.c -o brainfuck

clean:
	rm brainfuck
