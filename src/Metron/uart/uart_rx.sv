`ifndef UART_RX_SV
`define UART_RX_SV
`default_nettype none

//==============================================================================

module uart_rx
(
  input logic clk_4x,
  input logic rst_n,
  input logic in_serial,
  output logic[7:0] out_data,
  output logic out_valid
);

  logic [1:0] cycle_count;
  logic [3:0] bit_count;

  always @(posedge clk_4x, negedge rst_n) begin
    if (!rst_n) begin
      cycle_count <= 0;
      bit_count <= 0;
      out_data <= 0;
      out_valid <= 0;
    end
    else begin
      if (cycle_count != 0) begin
        cycle_count <= cycle_count - 1;
        out_valid <= 0;
      end
      else if (bit_count != 0) begin
        out_data <= {in_serial, out_data[7:1]};
        cycle_count <= 3;
        bit_count <= bit_count - 1;
        out_valid <= (bit_count == 2);
      end
      else if (in_serial == 0) begin
        cycle_count <= 3;
        bit_count <= 9;
        out_data <= 8'bxxxxxxxx;
        out_valid <= 0;
      end
    end
  end

endmodule

//==============================================================================

`endif
