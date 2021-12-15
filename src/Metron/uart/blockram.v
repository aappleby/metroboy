//=============================================================================
// tiny one-read-port + one-write-port memory to simulate block ram

module blockmem(clk, read_en, raddr, rdata, write_en, waddr, wdata);
  parameter WORD_COUNT = 32;
  parameter INIT_FILE = "";

  input clk;

  input read_en;
  input [$clog2(WORD_COUNT) - 1:0] raddr;
  output reg [31:0] rdata;

  input write_en;
  input [$clog2(WORD_COUNT) - 1:0] waddr;
  input [31:0] wdata;

  (* RAM_STYLE = "block" *)
  reg [31:0] memory [0:WORD_COUNT-1];

  always @(posedge clk) begin
    if (write_en) begin
      memory[waddr] <= wdata;
    end

    if (read_en) begin
      rdata <= memory[raddr];
    end
  end

  initial begin : BLOCKMEM_INIT
    integer i;
    if (INIT_FILE != 0) begin
      $readmemh(INIT_FILE, memory);
    end else begin
      for (i = 0; i < WORD_COUNT; i = i + 1) memory[i] = 0;
      memory[32*0] = 16*0;
      memory[32*1] = 16*1;
      memory[32*2] = 16*2;
      memory[32*3] = 16*3;
    end
  end
endmodule
