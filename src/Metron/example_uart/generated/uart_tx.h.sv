
`include "metron_tools.h.sv"

//==============================================================================

module uart_tx
#(parameter int cycles_per_bit = 4)
(
  input logic clock,
  input logic i_rstn,
  input logic[7:0] i_data,
  input logic i_req,
  output logic o_serial,
  output logic o_cts,
  output logic o_idle
);
 /*public:*/
  /*verilator public_module*/

  // 1 start bit, 8 data bits, 1 stop bit, 7 additional stop bits to guarantee
  // that recevier can resync between messages
  localparam /*const*/ int extra_stop_bits = 7;
  localparam /*const*/ int cycle_bits = $clog2(cycles_per_bit);
  localparam /*const*/ int cursor_bits = $clog2(10 + extra_stop_bits);

  logic[cycle_bits-1:0] cycle;
  logic[cursor_bits-1:0] cursor;
  logic[8:0] buffer;

  /*logic<1> o_serial;*/
  /*logic<1> o_cts;*/
  /*logic<1> o_idle;*/

  //----------------------------------------

  always_ff @(posedge clock) begin : tick
    if (!i_rstn) begin
      cycle <= 0;
      cursor <= 0;
      buffer <= 12'h1FF;
    end else begin
      logic[cycle_bits-1:0] cycle_max;
      logic[cursor_bits-1:0] cursor_max;
      cycle_max = cycle_bits'(cycles_per_bit - 1);
      cursor_max = cursor_bits'(10 + extra_stop_bits - 1);

      if (cursor <= extra_stop_bits && cycle == 0 && i_req) begin
        // Transmit start
        cycle <= cycle_max;
        cursor <= cursor_max;
        buffer <= i_data << 1;
      end else if (cycle != 0) begin
        // Bit delay
        cycle <= cycle - 1;
        cursor <= cursor;
        buffer <= buffer;
      end else if (cursor != 0) begin
        // Bit delay done, switch to next bit.
        cycle <= cycle_max;
        cursor <= cursor - 1;
        buffer <= (buffer >> 1) | 12'h100;
      end
    end
  end

  //----------------------------------------

  always_comb begin : tock
    o_serial = buffer & 1;
    o_cts = ((cursor == extra_stop_bits) && (cycle == 0)) ||
            (cursor < extra_stop_bits);
    o_idle = (cursor == 0) && (cycle == 0);
  end
endmodule

//==============================================================================

