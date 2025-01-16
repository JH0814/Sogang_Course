
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/10/07 21:58:27
// Design Name: 
// Module Name: Half_Subst_tb
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

`timescale 1ns / 1ps
module Half_Subst_tb;
reg aa, bb;
wire ss, cc;
Half_Subst u_test(
.a (aa),
.b (bb),

.s (ss),
.c (cc)
);
initial begin
aa = 1'b0;
bb = 1'b0;
end
always@(aa or bb) begin
aa <= #50 ~aa;
bb <= #100 ~bb;
end
initial begin
#1000
$finish;
end
endmodule
