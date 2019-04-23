MetroBoy: $(wildcard src/*.cpp)
	clang++ -O3 -mbmi2 -std=c++11 -stdlib=libc++ -o $@ $^ -lSDL2

clean:
	rm MetroBoy

.PHONY: clean
