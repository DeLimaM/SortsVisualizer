CC = gcc
CFLAGS = -Wall -std=c99 -D_DEFAULT_SOURCE
INCLUDE = -Iinclude -Iextern/raylib/src
RAYLIB_DIR = extern/raylib
EXECUTABLE = sortsvisualizer

UNAME_S = $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    PLATFORM_LDFLAGS = -lGL
endif
ifeq ($(UNAME_S),Darwin)
    PLATFORM_LDFLAGS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo
endif

LDFLAGS = -lm -lpthread -ldl -lrt -lX11 $(PLATFORM_LDFLAGS)

all: build

raylib:
	@echo "Building raylib..."
	@cd $(RAYLIB_DIR)/src && $(MAKE) PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=STATIC GRAPHICS=GRAPHICS_API_OPENGL_33 USE_EXTERNAL_GLFW=FALSE RAYLIB_BUILD_MODE=RELEASE RAYLIB_RELEASE_PATH="." CFLAGS="-w -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33" -B

build: raylib
	mkdir -p build
	$(CC) src/*.c -o build/$(EXECUTABLE) $(CFLAGS) $(INCLUDE) $(RAYLIB_DIR)/src/libraylib.a $(LDFLAGS)

clean:
	rm -rf build $(EXECUTABLE)
	cd $(RAYLIB_DIR)/src && $(MAKE) clean

rebuild: clean build

install:
	[ -f build/$(EXECUTABLE) ] || (echo Please build the program first by running 'make build' && exit 1)
	sudo cp build/$(EXECUTABLE) /usr/bin/sortviz

uninstall:
	sudo rm -f /usr/bin/sortviz

reinstall: uninstall rebuild install

.PHONY: all raylib build clean rebuild run install uninstall reinstall