`default_nettype none

//==============================================================================

module uart_rx
#(parameter clocks_per_bit = 4)
(
  input logic clk,
  input logic rst_n,

  input  logic       i_sio,

  output logic[7:0]  o_data,
  output logic       o_valid,
  output logic[31:0] o_sum
);
  /*verilator public_module*/
  localparam timer_bits = $clog2(clocks_per_bit);
  logic[timer_bits-1:0] rx_cycle;
  logic[3:0]  rx_bit;
  logic[31:0] rx_sum;
  logic[7:0]  rx_data;

  task automatic reset();
    rx_cycle <= 0;
    rx_bit <= 0;
    rx_data <= 0;
    rx_sum <= 0;
  endtask

  always_comb begin
    o_data  = rx_data;
    o_valid = rx_bit == 1;
    o_sum   = rx_sum;
  end

  task automatic tock(logic i_sio);
    if (rx_cycle != 0) begin
      rx_cycle <= rx_cycle - 1;
    end
    else if (rx_bit != 0) begin
      rx_cycle <= timer_bits'(clocks_per_bit - 1);
      rx_bit <= rx_bit - 1;
      rx_data <= {i_sio, rx_data[7:1]};
      if (rx_bit == 2) rx_sum <= rx_sum + {24'b0, i_sio, rx_data[7:1]};
    end
    else if (i_sio == 0) begin
      rx_cycle <= timer_bits'(clocks_per_bit - 1);
      rx_bit <= 9;
    end
  endtask

  always_ff @(posedge clk, negedge rst_n) begin
    if (!rst_n) reset();
    else        tock(i_sio);
  end

endmodule

//==============================================================================
