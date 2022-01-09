//=============================================================================
// tiny one-read-port + one-write-port memory to simulate block ram

module blockram_128x32
(
  input logic clk,
  input logic rst_n,

  input logic        read_en,
  input logic[6:0]   raddr,
  output logic[31:0] rdata,

  input logic        write_en,
  input logic[6:0]   waddr,
  input logic[31:0]  wdata
);
  localparam size = 128;
  reg [31:0] memory[size];

  initial begin
  end

  always @(posedge clk) begin
    if (write_en) memory[waddr] <= wdata;
    if (read_en)  rdata <= memory[raddr];
  end

endmodule
