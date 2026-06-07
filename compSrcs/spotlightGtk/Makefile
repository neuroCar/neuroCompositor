build: main.cpp
	clang++ main.cpp win.cc row.cc -o spotlight $(shell pkg-config --cflags --libs gtkmm-4.0)
clean:
	rm main

run:
	make build && ./main

all:
	make clean && make run