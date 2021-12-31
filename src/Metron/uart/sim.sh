#verilator --lint-only -Wall uart_bench.sv // not supported by verilator
#verilator --lint-only -Wall uart_hello.sv
#verilator --lint-only -Wall uart_ice40.sv
#verilator --lint-only -Wall uart_rx.sv
#verilator --lint-only -Wall uart_top.sv
#verilator --lint-only -Wall uart_tx.sv

mkdir -p bin
rm -f bin/uart_bench_iv
echo "iverilog..."
iverilog -g2012 uart_bench.sv -o bin/uart_test_iv
echo "running..."
bin/uart_test_iv
