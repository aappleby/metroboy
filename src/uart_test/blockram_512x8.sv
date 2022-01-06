//=============================================================================
// tiny one-read-port + one-write-port memory to simulate block ram

module blockram_512x8
#(parameter INIT_FILE = "")
(
  input logic clk,
  input logic rst_n,

  input logic[8:0]  i_addr,
  output logic[7:0] o_data
);
  localparam size = 512;

  (* RAM_STYLE = "block" *)
  reg [7:0] memory[size];
  logic[7:0] data;

  initial begin
    integer i;
    $readmemh(INIT_FILE, memory, 0, 511);
  end

  always_comb begin
    o_data = data;
  end

  task automatic reset();
    data <= '0;
  endtask

  task automatic tock(logic[8:0] i_addr);
    data <= memory[i_addr];
  endtask

  always_ff @(posedge clk, negedge rst_n) begin
    if (!rst_n) reset();
    else        tock(i_addr);
  end

endmodule
