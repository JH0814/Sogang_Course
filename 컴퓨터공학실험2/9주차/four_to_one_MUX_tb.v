`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/04 10:52:11
// Design Name: 
// Module Name: four_to_one_MUX_tb
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


module four_to_one_MUX_tb;
reg aa,bb,AA,BB,CC,DD;
wire XX;
four_to_one_MUX u_test(
.a (aa),
.b (bb),
.A (AA),
.B (BB),
.C (CC),
.D (DD),

.X (XX)
);
initial begin
aa = 1'b0;
bb = 1'b0;
AA = 1'b0;
BB = 1'b0;
CC = 1'b0;
DD = 1'b0;
end
always@(aa or bb or AA or BB or CC or DD) begin
aa <= #12.5 ~aa;
bb <= #25 ~bb;
AA <= #50 ~AA;
BB <= #100 ~BB;
CC <= #200 ~CC;
DD <= #400 ~DD;
end
initial begin
#2000
$finish;
end
endmodule
