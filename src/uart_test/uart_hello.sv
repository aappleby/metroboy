/* verilator lint_off WIDTH */
`default_nettype none
`timescale 1 ns / 1 ns

//==============================================================================

module uart_hello (
  input logic clk,
  input logic rst_n,

  input logic i_cts,
  input logic i_idle,

  output logic [7:0] o_data,
  output logic       o_req,
  output logic       o_done
);
  //----------------------------------------
  /*verilator public_module*/

  //localparam message_len = 7;
  localparam message_len = 512;
  localparam cursor_bits = $clog2(message_len);

  typedef enum logic[2:0] { WAIT, SEND, DONE } e_state;
  e_state state;
  logic[cursor_bits-1:0] cursor;

  //----------------------------------------

  logic[7:0] memory[512];
  logic[7:0] data;

  initial begin
    $readmemh("obj/message.hex", memory, 0, 511);
  end

  //----------------------------------------

  always_comb begin : tick
    o_data = data;
    o_req  = state == SEND;
    o_done = state == DONE;
  end

  //----------------------------------------

  always_ff @(posedge clk, negedge rst_n) begin : tock
    if (!rst_n) begin
      state <= WAIT;
      cursor <= 0;
    end else begin
      data <= memory[cursor];
      if (state == WAIT && i_idle) begin
        state <= SEND;
      end else if (state == SEND && i_cts) begin
        if (cursor == (cursor_bits)'(message_len - 1)) state <= DONE;
        cursor <= cursor + 1;
      end else if (state == DONE) begin
        state <= WAIT;
        cursor <= 0;
      end
    end
  end

  //----------------------------------------

/*
  void dump_header() {
    printf("[hlo state cursor data req done] ");
  }

  void dump() {
    printf("[    %-5d %-6d %04x %-3d %-4d] ", state, cursor, o_data, o_req, o_done);
  }

  void dump_vcd_header(VcdDump& d) {
    fprintf(d.file, "$scope module hello $end\n");
    fprintf(d.file, "$var wire 2 hello_state  state  $end\n");
    fprintf(d.file, "$var wire 9 hello_cursor cursor $end\n");
    fprintf(d.file, "$var wire 8 hello_o_data o_data $end\n");
    fprintf(d.file, "$var wire 1 hello_o_req  o_req  $end\n");
    fprintf(d.file, "$var wire 1 hello_o_done o_done $end\n");
    fprintf(d.file, "$upscope $end\n");
  }

  void dump_value(VcdDump& d) {
    d.set_value("hello_state",  state,  2);
    d.set_value("hello_cursor", cursor, 9);
    d.set_value("hello_o_data", o_data, 8);
    d.set_value("hello_o_req",  o_req,  1);
    d.set_value("hello_o_done", o_done, 1);
  }
*/
endmodule

//==============================================================================
