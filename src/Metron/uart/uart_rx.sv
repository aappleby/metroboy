`timescale 1 ns / 1 ps

//==============================================================================

module uart_rx(input clk, input rst_n, input in_serial, output logic [7:0] out_result, output logic out_result_valid);
  parameter clocks_per_bit = 4;

  reg [$clog2(clocks_per_bit)-1:0] cycle_count;
  reg [3:0] bit_count;

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      cycle_count <= 0;
      bit_count <= 0;
      out_result <= 0;
      out_result_valid <= 0;
    end else begin
      if (cycle_count != 0) begin
        cycle_count <= cycle_count - 1;
      end
      else if (bit_count != 0) begin
        out_result <= {in_serial, out_result[7:1]};
        cycle_count <= (clocks_per_bit - 1);
        bit_count <= bit_count - 1;

      end else begin
        if (in_serial == 0) begin
          cycle_count <= clocks_per_bit - 1; //clocks_per_bit / 2 - 2;
          bit_count <= 9;
          out_result <= 8'bxxxxxxxx;
        end
      end

      out_result_valid <= (bit_count == 2) && (cycle_count == 0);
    end
  end

endmodule

//==============================================================================
