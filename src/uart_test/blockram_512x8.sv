//=============================================================================
// tiny one-read-port + one-write-port memory to simulate block ram

module blockram_512x8
(
  input logic clk,
  input logic rst_n,

  input logic[8:0]  i_addr,
  output logic[7:0] o_data
);
  localparam size = 512;
  reg [7:0] memory[size];
  logic[8:0] addr;

  //----------------------------------------

  initial begin
  end

  task automatic reset();
    addr <= '0;
  endtask

  always_comb begin
    o_data = memory[addr];
  end

  task automatic tock();
    addr <= i_addr;
  endtask

  //----------------------------------------

  always_ff @(posedge clk, negedge rst_n) begin
    if (!rst_n) reset();
    else        tock();
  end

endmodule

//=============================================================================
