set -x
verilator -Wno-fatal --cc toplevel.h.sv --Mdir .
g++ -I /usr/share/verilator/include -I /usr/share/verilator/include/vltstd -c -o Vtoplevel___024root__Slow.o Vtoplevel___024root__Slow.cpp
g++ -I /usr/share/verilator/include -I /usr/share/verilator/include/vltstd -c -o Vtoplevel__ConstPool_0.o Vtoplevel__ConstPool_0.cpp
g++ -I /usr/share/verilator/include -I /usr/share/verilator/include/vltstd -c -o Vtoplevel__Syms.o Vtoplevel__Syms.cpp
g++ -I /usr/share/verilator/include -I /usr/share/verilator/include/vltstd -c -o Vtoplevel___024unit__Slow.o Vtoplevel___024unit__Slow.cpp
g++ -I /usr/share/verilator/include -I /usr/share/verilator/include/vltstd -c -o Vtoplevel___024unit__DepSet_h1dabdd97__0__Slow.o Vtoplevel___024unit__DepSet_h1dabdd97__0__Slow.cpp
g++ -I /usr/share/verilator/include -I /usr/share/verilator/include/vltstd -c -o Vtoplevel___024root__DepSet_h9890b585__0.o Vtoplevel___024root__DepSet_h9890b585__0.cpp
g++ -I /usr/share/verilator/include -I /usr/share/verilator/include/vltstd -c -o Vtoplevel___024root__DepSet_h9890b585__0__Slow.o Vtoplevel___024root__DepSet_h9890b585__0__Slow.cpp
g++ -I /usr/share/verilator/include -I /usr/share/verilator/include/vltstd -c -o main.o main.cpp
g++ -I /usr/share/verilator/include -I /usr/share/verilator/include/vltstd -c -o Vtoplevel.o Vtoplevel.cpp
g++ -I /usr/share/verilator/include -I /usr/share/verilator/include/vltstd Vtoplevel___024root__Slow.o Vtoplevel__ConstPool_0.o Vtoplevel__Syms.o Vtoplevel___024unit__Slow.o Vtoplevel___024unit__DepSet_h1dabdd97__0__Slow.o Vtoplevel___024root__DepSet_h9890b585__0.o Vtoplevel___024root__DepSet_h9890b585__0__Slow.o main.o Vtoplevel.o /usr/share/verilator/include/verilated.cpp -o testbench
./testbench +text_file=tests/sh.text.vh +data_file=tests/sh.data.vh
./testbench +text_file=tests/sub.text.vh +data_file=tests/sub.data.vh
./testbench +text_file=tests/lh.text.vh +data_file=tests/lh.data.vh
./testbench +text_file=tests/sltu.text.vh +data_file=tests/sltu.data.vh
./testbench +text_file=tests/sra.text.vh +data_file=tests/sra.data.vh
./testbench +text_file=tests/beq.text.vh +data_file=tests/beq.data.vh
./testbench +text_file=tests/auipc.text.vh +data_file=tests/auipc.data.vh
./testbench +text_file=tests/blt.text.vh +data_file=tests/blt.data.vh
./testbench +text_file=tests/bne.text.vh +data_file=tests/bne.data.vh
./testbench +text_file=tests/slli.text.vh +data_file=tests/slli.data.vh
./testbench +text_file=tests/lbu.text.vh +data_file=tests/lbu.data.vh
./testbench +text_file=tests/simple.text.vh +data_file=tests/simple.data.vh
./testbench +text_file=tests/sb.text.vh +data_file=tests/sb.data.vh
./testbench +text_file=tests/bge.text.vh +data_file=tests/bge.data.vh
./testbench +text_file=tests/sltiu.text.vh +data_file=tests/sltiu.data.vh
./testbench +text_file=tests/jal.text.vh +data_file=tests/jal.data.vh
./testbench +text_file=tests/srli.text.vh +data_file=tests/srli.data.vh
./testbench +text_file=tests/and.text.vh +data_file=tests/and.data.vh
./testbench +text_file=tests/slt.text.vh +data_file=tests/slt.data.vh
./testbench +text_file=tests/xori.text.vh +data_file=tests/xori.data.vh
./testbench +text_file=tests/xor.text.vh +data_file=tests/xor.data.vh
./testbench +text_file=tests/or.text.vh +data_file=tests/or.data.vh
./testbench +text_file=tests/andi.text.vh +data_file=tests/andi.data.vh
./testbench +text_file=tests/sll.text.vh +data_file=tests/sll.data.vh
./testbench +text_file=tests/srai.text.vh +data_file=tests/srai.data.vh
./testbench +text_file=tests/lb.text.vh +data_file=tests/lb.data.vh
./testbench +text_file=tests/sw.text.vh +data_file=tests/sw.data.vh
./testbench +text_file=tests/bgeu.text.vh +data_file=tests/bgeu.data.vh
./testbench +text_file=tests/jalr.text.vh +data_file=tests/jalr.data.vh
./testbench +text_file=tests/lw.text.vh +data_file=tests/lw.data.vh
./testbench +text_file=tests/lui.text.vh +data_file=tests/lui.data.vh
./testbench +text_file=tests/srl.text.vh +data_file=tests/srl.data.vh
./testbench +text_file=tests/add.text.vh +data_file=tests/add.data.vh
./testbench +text_file=tests/lhu.text.vh +data_file=tests/lhu.data.vh
./testbench +text_file=tests/ori.text.vh +data_file=tests/ori.data.vh
./testbench +text_file=tests/slti.text.vh +data_file=tests/slti.data.vh
./testbench +text_file=tests/addi.text.vh +data_file=tests/addi.data.vh
./testbench +text_file=tests/bltu.text.vh +data_file=tests/bltu.data.vh

