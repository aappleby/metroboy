`ifndef VRAM_SV
`define VRAM_SV

`include "types.sv"
import types::*;

//-----------------------------------------------------------------------------

module VRAM (
  input bool clk,
  input bool rst,
  input CpuBus bus
);

parameter uint16_t ADDR_VRAM_BEGIN = 16'h8000;
parameter uint16_t ADDR_VRAM_END   = 16'h9FFF;

typedef logic [12:0] ram_addr_t;
uint8_t ram[8192];
BusOut out;

//-----------------------------------------------------------------------------

task reset;
  foreach(ram[i]) ram[i] <= 0;
  out.data <= 0;
  out.oe <= 0;
endtask;

//----------

function BusOut tick;
  return out;
endfunction

//----------

task tock(CpuBus bus);
  ram_addr_t ram_addr = ram_addr_t'(bus.addr - ADDR_VRAM_BEGIN);

  out.data <= 0;
  out.oe <= 0;
  if (bus.addr < ADDR_VRAM_BEGIN || bus.addr> ADDR_VRAM_END) return;
  if (bus.read)  out <= {ram[ram_addr], true};
  if (bus.write) ram[ram_addr] <= bus.data;
endtask;

//----------

always @(posedge clk) begin
  if (rst) reset();
  else tock(bus);
end

endmodule;

//-----------------------------------------------------------------------------

`endif // VRAM_SV