build:
	mkdir -p build
	cd build && cmake ..
	cd build && make

run:
	cd build && ./checkmate_solver

all: build run

clean:
	rm -rf build
