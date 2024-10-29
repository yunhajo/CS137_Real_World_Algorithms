all: bin/to_ppm bin/to_jpeg

bin/to_ppm: src/JPEG_decoder.cpp
	g++ -O2 src/JPEG_decoder.cpp -o bin/to_ppm -std=c++17

bin/to_jpeg: src/JPEG_encoder.cpp
	g++ -O2 src/JPEG_encoder.cpp -o bin/to_jpeg -std=c++17

clean:
	rm -f bin/*
.PHONY: clean
