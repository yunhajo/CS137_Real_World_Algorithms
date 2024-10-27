all: bin/myunzip bin/myzip

bin/myunzip: src/myunzip.cpp
	g++ -O2 src/myunzip.cpp src/inflate8.cpp -o bin/myunzip -std=c++17

bin/myzip: src/myzip.cpp
	g++ -O2 src/myzip.cpp src/deflate8.cpp -o bin/myzip -std=c++17

bin/%: src/%.cpp
	g++ -02 $< -o $@ -std=c++17

clean:
	rm -f bin/*
.PHONY: clean
