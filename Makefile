all:
	mkdir -p build
	cd build && cmake ..
	cd build && make
	cd build && ./checkmate_solver
