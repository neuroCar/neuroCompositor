build: ./src/main.cpp
	clang++ ./src/main.cpp ./src/win.cc -o wallpaperGtk $(shell pkg-config --cflags --libs gtkmm-4.0 gtk4-layer-shell-0)

clean:
	rm wallpaperGtk

run:
	make build && ./wallpaperGtk

all:
	make clean && make run