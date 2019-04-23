SOURCES = $(wildcard src/*.cpp)
HEADERS = $(wildcard src/*.h)
OBJECTS = $(SOURCES:.cpp=.o)

MetroBoy: $(SOURCES:.cpp=.o)
	$(CXX) -o $@ $^ -lSDL2

%.o: %.cpp
	$(CXX) -Wall -Wextra -Weffc++ -O3 -mbmi2 -std=c++14 -c $< -o $@

clean:
	rm $(OBJECTS)
	rm MetroBoy

.PHONY: clean
