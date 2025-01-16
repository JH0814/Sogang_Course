
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/10/07 21:43:14
// Design Name: 
// Module Name: Full_Adder_tb
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
module Full_Adder_tb;
reg aa, bb, cc;
wire ss, oo;
Full_Adder u_test(
.a (aa),
.b (bb),
.c (cc),

.s (ss),
.cout (oo)
);
initial begin
aa = 1'b0;
bb = 1'b0;
cc = 1'b0;
end
always@(aa or bb or cc) begin
aa <= #50 ~aa;
bb <= #100 ~bb;
cc <= #100 ~cc;
end
initial begin
#1000
$finish;
end
endmodule
