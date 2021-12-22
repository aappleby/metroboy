rm out/*
echo "########## yosys"
time yosys -p 'read_verilog -sv uart_ice40.sv; dump; synth_ice40 -json out/uart_ice40.json;'
echo "########## nextpnr"
time nextpnr-ice40 -q --hx8k --package ct256 --json out/uart_ice40.json --asc out/uart_ice40.asc --pcf ice40-hx8k-b-evn.pcf
echo "########## icepack"
icepack out/uart_ice40.asc out/uart_ice40.bin
echo "########## iceprog"
iceprog -S out/uart_ice40.bin
