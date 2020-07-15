build/a.out: src/buffer.c src/node.c src/options.c src/print.c src/main.c
	gcc -o build/a.out src/buffer.c src/node.c src/options.c src/print.c src/main.c -Isrc

.PHONY: clean

clean:
	rm build/*
