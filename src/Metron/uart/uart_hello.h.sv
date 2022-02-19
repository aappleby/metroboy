`default_nettype none


`include "../metron_tools.h.sv"

//==============================================================================
/* verilator lint_off WIDTH */

module uart_hello
(
input logic clk,
input logic rst_n,

input bool i_cts,
input bool i_idle,
output logic[7:0] o_data,
output logic o_req,
output logic o_done
); 

  localparam int message_len = 512;
  localparam int cursor_bits = $clog2(message_len);

  typedef enum logic[1:0] {
    WAIT, SEND, DONE
  } state;

  state s;
  logic[cursor_bits-1:0] cursor;
  logic[7:0] memory[512];
  logic[7:0] data;

  
  
  

  //----------------------------------------

  initial begin : init
    $readmemh("obj/message.hex", memory, 0, 511);
    o_data = 0;
    o_req = 0;
    o_done = 0;
  end

  //----------------------------------------

  always_ff @(posedge clk, negedge rst_n) begin : tick
    if (!rst_n) begin
      s <= WAIT;
      cursor <= 0;
    end
    else begin
      data <= memory[cursor];
      if (s == WAIT && i_idle) begin
        s <= SEND;
      end
      else if (s == SEND && i_cts) begin
        if (cursor == 9'(message_len - 1)) begin
          s <= DONE;
        end
        else begin
          cursor <= cursor + 1;
        end
      end
      else if (s == DONE) begin
        //s = state::WAIT;
        cursor <= 0;
      end
    end
  end

  //----------------------------------------

  always_comb begin : tock
    o_data = data;
    o_req  = (s == SEND);
    o_done = (s == DONE);
  end

endmodule

//==============================================================================
