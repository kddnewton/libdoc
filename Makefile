build/a.out: build src/buffer.c src/debug.c src/memory.c src/node.c src/options.c src/print.c src/stack.c src/main.c
	gcc -o build/a.out src/buffer.c src/debug.c src/memory.c src/node.c src/options.c src/print.c src/stack.c src/main.c -Isrc -Wall -Wextra

build:
	mkdir -p build

.PHONY: clean

clean:
	rm build/*
