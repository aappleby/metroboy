
VERILATOR_INCLUDE=/usr/share/verilator/include
SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)
CXXFLAGS = -O3 -I ${VERILATOR_INCLUDE} -I ${VERILATOR_INCLUDE}/vltstd
SV_SOURCES=$(wildcard *.sv)

build: Vtoplevel.h main.cpp
	${MAKE} run

Vtoplevel.h: $(SV_SOURCES)
	verilator -I../.. -O3 --x-assign fast --x-initial fast --noassert -Wno-fatal --cc toplevel.sv --Mdir .

run: testbench
	./testbench

testbench: ${OBJS}
	${CXX} ${CXXFLAGS} ${OBJS} ${VERILATOR_INCLUDE}/verilated.cpp -o testbench

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $<

clean:
	rm -f testbench ${OBJS} $(wildcard V*)

