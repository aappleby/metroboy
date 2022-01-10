`default_nettype none
/* verilator lint_off WIDTH */

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

  localparam cycle_bits  = $clog2(cycles_per_bit);
  localparam cycle_max   = cycles_per_bit - 1;
  localparam cursor_max  = 9;
  localparam cursor_bits = $clog2(cursor_max);

  logic[cycle_bits-1:0]   cycle;
  logic[cursor_bits-1:0]  cursor;
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
      logic [7:0] temp;
      temp = {i_serial, buffer[7:1]};
      cycle  <= cycle_max;
      cursor <= cursor - 1;
      buffer <= temp;
      if ((cursor - 1) == 1) sum <= sum + temp;
    end
    else if (i_serial == 0) begin
      cycle  <= cycle_max;
      cursor <= cursor_max;
    end
  endtask

  //----------------------------------------


























  always_ff @(posedge clk, negedge rst_n) begin
    if (!rst_n) reset();
    else        tock(i_serial);
  end

endmodule

//==============================================================================
