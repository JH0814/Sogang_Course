
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/10/14 10:41:35
// Design Name: 
// Module Name: Parity_Gene_tb
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
module Parity_Gene_tb;
reg aa, bb, cc, dd;
wire ee;
Parity_Gene u_test(
.a (aa),
.b (bb),
.c (cc),
.d (dd),

.e (ee)
);
initial begin
aa = 1'b0;
bb = 1'b0;
cc = 1'b0;
dd = 1'b0;
end
always@(aa or bb or cc or dd) begin
aa <= #50 ~aa;
bb <= #100 ~bb;
cc <= #200 ~cc;
dd <= #400 ~dd;
end
initial begin
#1000
$finish;
end
endmodule
