#verilator --lint-only -Wall uart_bench.sv // not supported by verilator
verilator --lint-only -Wall uart_hello.sv
verilator --lint-only -Wall uart_ice40.sv
verilator --lint-only -Wall uart_rx.sv
verilator --lint-only -Wall uart_top.sv
verilator --lint-only -Wall uart_tx.sv

rm -f out/a.out
iverilog -g2012 uart_bench.sv -o out/a.out; out/a.out;

