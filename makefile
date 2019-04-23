MetroBoy: $(wildcard src/*.cpp)
	$(CXX) -O3 -mbmi2 -std=c++11 -o $@ $^ -lSDL2

clean:
	rm MetroBoy

.PHONY: clean
