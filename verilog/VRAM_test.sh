echo Cleaning
rm -rf obj_dir
echo Verilating
verilator -Wall --cc VRAM.sv
echo Compiling
g++ -I/usr/local/share/verilator/include -o VRAM_test VRAM_test.cpp obj_dir/VVRAM.cpp obj_dir/VVRAM__Syms.cpp /usr/local/share/verilator/include/verilated.cpp
echo Running
./VRAM_test

