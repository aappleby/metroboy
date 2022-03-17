
`include "metron_tools.h.sv"

//==============================================================================

module uart_rx
#(parameter int cycles_per_bit = 4)
(
  input logic clock,
  input logic i_rstn,
  input logic i_serial,
  output logic[7:0] o_data,
  output logic o_valid,
  output logic[31:0] o_sum
);
 /*public:*/
  /*verilator public_module*/

  localparam /*const*/ int cycle_bits = $clog2(cycles_per_bit);
  localparam /*const*/ int cycle_max = cycles_per_bit - 1;
  localparam /*const*/ int cursor_max = 9;
  localparam /*const*/ int cursor_bits = $clog2(cursor_max);

  logic[cycle_bits-1:0] cycle;
  logic[cursor_bits-1:0] cursor;
  logic[7:0] buffer;
  logic[31:0] sum;

  /*logic<8> o_data;*/
  /*logic<1> o_valid;*/
  /*logic<32> o_sum;*/

  //----------------------------------------

  always_ff @(posedge clock) begin : tick
    if (!i_rstn) begin
      cycle <= 0;
      cursor <= 0;
      buffer <= 0;
      sum <= 0;
    end else begin
      if (cycle != 0) begin
        cycle <= cycle - 1;
      end else if (cursor != 0) begin
        logic[7:0] temp;
        temp = (i_serial << 7) | (buffer >> 1);
        if (cursor - 1 == 1) sum <= sum + temp;
        cycle <= cycle_max;
        cursor <= cursor - 1;
        buffer <= temp;
      end else if (i_serial == 0) begin
        cycle <= cycle_max;
        cursor <= cursor_max;
      end
    end
  end

  //----------------------------------------

  always_comb begin : tock
    o_data = buffer;
    o_valid = cursor == 1;
    o_sum = sum;
  end

endmodule

//==============================================================================
