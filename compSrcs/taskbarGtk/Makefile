build: ./src/main.cpp
	clang++ ./src/main.cpp ./src/win.cc ./src/btn.cc ./src/row.cc -o taskbarGtk -std=c++23 $(shell pkg-config --cflags --libs gtkmm-4.0 gtk4-layer-shell-0 sdbus-c++)

clean:
	rm taskbarGtk

run:
	make build && ./taskbarGtk

all:
	make clean && make run