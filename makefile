MetroBoy: $(wildcard src/*.cpp)
	$(CXX) -Wall -Wextra -Weffc++ -O3 -mbmi2 -std=c++14 $^ -o $@ -lSDL2
