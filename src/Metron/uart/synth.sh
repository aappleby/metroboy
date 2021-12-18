clear
echo iverilog
iverilog -g2012 uart_bench.sv; ./a.out;

echo verilator
verilator --cc uart_top.sv

echo yosys
yosys -q -p 'read_verilog -sv uart_top.sv; synth_ice40 -json uart_top.json'

echo nextpnr
nextpnr-ice40 -q --hx1k --json uart_top.json --pcf uart_top.pcf --asc uart_top.asc --package tq144

echo icepack
icepack uart_top.asc uart_top.bin

