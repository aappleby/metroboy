`default_nettype none

//==============================================================================

module uart_rx
#(parameter cycles_per_bit = 4)
(
  input logic clk,
  input logic rst_n,

  input  logic       i_serial,

  output logic[7:0]  o_data,
  output logic       o_valid,
  output logic[31:0] o_sum
);
  /*verilator public_module*/

  //----------------------------------------

  localparam cycle_bits = $clog2(cycles_per_bit);
  logic[cycle_bits-1:0] cycle;
  logic[3:0]  cursor;
  logic[7:0]  buffer;
  logic[31:0] sum;

  //----------------------------------------

  task automatic reset();
    cycle  <= 0;
    cursor <= 0;
    buffer <= 0;
    sum    <= 0;
  endtask

  //----------------------------------------

  always_comb begin : tick
    o_data  = buffer;
    o_valid = cursor == 1;
    o_sum   = sum;
  end

  //----------------------------------------

  task automatic tock(logic i_serial);
    if (cycle != 0) begin
      cycle  <= cycle - 1;
    end
    else if (cursor != 0) begin
      cycle  <= cycle_bits'(cycles_per_bit - 1);
      cursor <= cursor - 1;
      buffer <= {i_serial, buffer[7:1]};
      if (cursor == 2) sum <= sum + {24'b0, i_serial, buffer[7:1]};
    end
    else if (i_serial == 0) begin
      cycle  <= cycle_bits'(cycles_per_bit - 1);
      cursor <= 9;
    end
  endtask

  //----------------------------------------


























  always_ff @(posedge clk, negedge rst_n) begin
    if (!rst_n) reset();
    else        tock(i_serial);
  end

endmodule

//==============================================================================
