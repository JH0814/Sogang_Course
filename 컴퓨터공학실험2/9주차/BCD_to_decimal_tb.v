
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/03 15:05:23
// Design Name: 
// Module Name: BCD_to_decimal_tb
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
module BCD_to_decimal_tb;
reg aa, bb, cc, dd;
wire dd11, dd22, dd33, dd44, dd55, dd66, dd77, dd88, dd99;
BCD_to_decimal u_test(
.A0 (aa),
.A1 (bb),
.A2 (cc),
.A3 (dd),

.D1 (dd11),
.D2 (dd22),
.D3 (dd33),
.D4 (dd44),
.D5 (dd55),
.D6 (dd66),
.D7 (dd77),
.D8 (dd88),
.D9 (dd99)
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
