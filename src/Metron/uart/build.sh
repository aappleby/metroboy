rm -f obj/uart_ice40.*
rm -f bin/uart_ice40.*
mkdir -p obj
mkdir -p bin
echo "########## yosys"
time yosys -p 'read_verilog -sv uart_ice40.sv; dump; synth_ice40 -json obj/uart_ice40.json;'
echo "########## nextpnr"
time nextpnr-ice40 -q --hx8k --package ct256 --json obj/uart_ice40.json --asc obj/uart_ice40.asc --pcf ice40-hx8k-b-evn.pcf
echo "########## icepack"
icepack obj/uart_ice40.asc bin/uart_ice40.bin
echo "########## iceprog"
iceprog -S bin/uart_ice40.bin
