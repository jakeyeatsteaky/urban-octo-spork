build_and_run:
	g++ -std=c++17 -Wall -g main.cpp -o jake_gl
	./jake_gl

clean:
	rm jake_gl

open:
	output.ppm

