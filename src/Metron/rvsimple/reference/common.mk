
VERILATOR_INCLUDE=/usr/share/verilator/include
SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)
CXXFLAGS=-I ${VERILATOR_INCLUDE} -I ${VERILATOR_INCLUDE}/vltstd
SV_SOURCES=$(wildcard ../../core/common/*.sv) $(wildcard ../../core/$(CORETYPE)/*.sv) ../config.sv
VFLAGS=-Wno-fatal -I.. -I../../core/common/ -I../../core/$(CORETYPE)
TESTDIR=../../tests
TESTS=$(notdir $(patsubst %.S,%,$(wildcard $(TESTDIR)/*.S)))

build: Vtoplevel.h main.cpp
	${MAKE} run

Vtoplevel.h: $(SV_SOURCES)
	verilator ${VFLAGS} --cc ../../core/$(CORETYPE)/toplevel.sv --Mdir .

run: $(addsuffix .run,$(TESTS))

%.run: testbench
	./testbench +text_file=$(TESTDIR)/$(@:.run=).text.vh +data_file=$(TESTDIR)/$(@:.run=).data.vh

testbench: ${OBJS}
	${CXX} ${CXXFLAGS} ${OBJS} ${VERILATOR_INCLUDE}/verilated.cpp -o testbench

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $<

main.cpp: ../main.cpp
	cp ../main.cpp .

clean:
	rm -f testbench main.cpp ${OBJS} $(wildcard V*)

