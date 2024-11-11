all: build

build:
	mkdir -p build
	clang src/*.c -o build/sortviz

install:
	[ -f build/sortviz ] || (echo Please build the program first by running 'make build' && exit 1)
	sudo cp build/sortviz /usr/bin/sortviz

uninstall:
	sudo rm -f /usr/bin/sortviz