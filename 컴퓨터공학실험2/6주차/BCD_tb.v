
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/10/07 22:20:18
// Design Name: 
// Module Name: BCD_tb
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
module BCD_tb;
reg pp, qq, rr, ss;
wire aa,bb,cc,dd;
BCD u_test(
.p (pp),
.q (qq),
.r (rr),
.s (ss),

.a (aa),
.b (bb),
.c (cc),
.d (dd)
);
initial begin
pp = 1'b0;
qq = 1'b0;
rr = 1'b0;
ss = 1'b0;
end
always@(pp or qq or rr or ss) begin
pp <= #50 ~pp;
qq <= #100 ~qq;
rr <= #150 ~rr;
ss <= #200 ~ss;
end
initial begin
#1000
$finish;
end
endmodule
