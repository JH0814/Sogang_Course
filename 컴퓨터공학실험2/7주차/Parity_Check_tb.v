
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/10/14 10:54:33
// Design Name: 
// Module Name: Parity_Check_tb
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
module Parity_Check_tb;
reg aa, bb, cc, dd, pp;
wire ppec;
Parity_Check u_test(
.a (aa),
.b (bb),
.c (cc),
.d (dd),
.p (pp),

.pec (ppec)
);
initial begin
aa = 1'b0;
bb = 1'b0;
cc = 1'b0;
dd = 1'b0;
pp = 1'b0;
end
always@(aa or bb or cc or dd or pp) begin
aa <= #50 ~aa;
bb <= #100 ~bb;
cc <= #200 ~cc;
dd <= #400 ~dd;
pp <= #800 ~pp;
end
initial begin
#1000
$finish;
end
endmodule
