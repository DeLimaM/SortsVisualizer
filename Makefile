all: build

build:
	mkdir -p build
	gcc src/*.c -Iinclude -Llib -o build/sortviz -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

clean:
	rm -rf build

rebuild: clean build

install:
	[ -f build/sortviz ] || (echo Please build the program first by running 'make build' && exit 1)
	sudo cp build/sortviz /usr/bin/sortviz

uninstall:
	sudo rm -f /usr/bin/sortviz

reinstall: uninstall rebuild install