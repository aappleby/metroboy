module IRAM(input types::bool rst, input types::bool clk, input types::CpuBus in_bus);
  import types::*;

  parameter uint16_t ADDR_IRAM_BEGIN = 16'hC000;
  parameter uint16_t ADDR_IRAM_END   = 16'hDFFF;
  parameter uint16_t ADDR_ECHO_BEGIN = 16'hE000;
  parameter uint16_t ADDR_ECHO_END   = 16'hFFFF;

  typedef logic [12:0] ram_addr_t;
  uint8_t ram[8192];
  BusOut out;

  task reset;
    for (integer i = 0; i < 8192; i++) ram[i] <= 0;
    out.data <= 0;
    out.oe <= 0;
  endtask;

  function BusOut tick;
    return out;
  endfunction

  task tock(CpuBus bus);
    ram_addr_t ram_addr = ram_addr_t'(bus.addr - ADDR_IRAM_BEGIN);
    out.data <= 0;
    out.oe <= 0;
    if (bus.addr < ADDR_IRAM_BEGIN) return;
    if (bus.read)  out <= {ram[ram_addr], true};
    if (bus.write) ram[ram_addr] <= bus.data;
  endtask;

  always @(posedge clk) begin
    if (rst) reset();
    else tock(in_bus);
  end

endmodule;
