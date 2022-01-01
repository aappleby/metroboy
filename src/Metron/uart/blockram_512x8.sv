//=============================================================================
// tiny one-read-port + one-write-port memory to simulate block ram

module blockram_512x8
#(parameter INIT_FILE = "")
(
  input logic clk,

  input logic       read_en,
  input logic[8:0]  raddr,
  output logic[7:0] rdata,

  input logic       write_en,
  input logic[8:0]  waddr,
  input logic[7:0]  wdata
);
  localparam size = 512;

  (* RAM_STYLE = "block" *)
  reg [7:0] memory[size];

  always @(posedge clk) begin
    if (write_en) memory[waddr] <= wdata;
    if (read_en)  rdata <= memory[raddr];
  end

  initial begin
    integer i;
    if (INIT_FILE != 0) begin
      $readmemh(INIT_FILE, memory, 0, 511);
    end else begin
      for (i = 0; i < size; i = i + 1) memory[i] = 0;
    end
  end

endmodule
