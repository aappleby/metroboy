@echo off
del /Q out\*.*
echo ##### yosys
yosys -p "synth_ice40 -json out/hardware.json" -q leds.v
echo ##### nextpnr
nextpnr-ice40 --hx8k --package ct256 --json out/hardware.json --asc out/hardware.asc --pcf leds.pcf -q
echo ##### icepack
icepack out/hardware.asc out/hardware.bin
echo ##### iceprog
iceprog -S out/hardware.bin
