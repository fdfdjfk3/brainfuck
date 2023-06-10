build: src/brainfuck.c
	gcc -O3 src/brainfuck.c -o brainfuck

clean:
	rm brainfuck
