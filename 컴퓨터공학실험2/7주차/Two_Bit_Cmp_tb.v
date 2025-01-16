
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/10/14 11:10:15
// Design Name: 
// Module Name: Two_Bit_Cmp_tb
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
module Two_Bit_Cmp_tb;
reg aa, bb, cc, dd;
wire ff1, ff2, ff3;
Two_Bit_Cmp u_test(
.a (aa),
.b (bb),
.c (cc),
.d (dd),

.f1 (ff1),
.f2 (ff2),
.f3 (ff3)
);
initial begin
aa = 1'b0;
bb = 1'b0;
cc = 1'b0;
dd = 1'b0;
end
always@(aa or bb or cc or dd) begin
aa <= #25 ~aa;
bb <= #50 ~bb;
cc <= #100 ~cc;
dd <= #200 ~dd;
end
initial begin
#1000
$finish;
end
endmodule
