`include "metron_top.sv"

//-----------------------------------------------------------------------------

module Life(input logic clk, input logic rst);
  
  logic grid[8][8];
  
  always @(posedge clk) begin
    
    int a, b, c, d, e, f, g, h, i, s;
    
    for (int y = 0; y < 8; y++) begin
      for (int x = 0; x < 8; x++) begin
        a = grid[(y - 1) & 7][(x - 1) & 7];
        b = grid[(y - 1) & 7][(x + 0) & 7];
        c = grid[(y - 1) & 7][(x + 1) & 7];
        d = grid[(y + 0) & 7][(x - 1) & 7];
        e = grid[(y + 0) & 7][(x + 1) & 7];
        f = grid[(y + 1) & 7][(x - 1) & 7];
        g = grid[(y + 1) & 7][(x + 0) & 7];
        h = grid[(y + 1) & 7][(x + 1) & 7];
        
        s = a + b + c + d + e + f + g + h;

        if (s <  2) grid[y][x] <= 0;
        if (s == 3) grid[y][x] <= 1;
        if (s >  3) grid[y][x] <= 0;
      end
    end
  end
 
endmodule

//-----------------------------------------------------------------------------
