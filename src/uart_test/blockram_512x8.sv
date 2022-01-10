//=============================================================================
// tiny one-read-port + one-write-port memory to simulate block ram

module blockram_512x8
(
  input logic clk,
  input logic rst_n,

  input logic[8:0]  i_addr,
  output logic[7:0] o_data
);
  /*verilator public_module*/

  localparam size = 512;
  
  reg [7:0] memory[size];
  logic[7:0] data;

  //----------------------------------------

  task automatic reset();
  endtask

  always_comb begin : tick
    o_data = data;
  end

  task automatic tock();
    data <= memory[i_addr];
  endtask

  //----------------------------------------

  initial begin
    $readmemh("obj/message.hex", memory, 0, 511);
  end

  always_ff @(posedge clk, negedge rst_n) begin
    if (!rst_n) reset();
    else        tock();
  end

endmodule

//=============================================================================
