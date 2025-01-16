`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/03 14:19:31
// Design Name: 
// Module Name: two_to_four_decoder_AND_tb
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


module two_to_four_decoder_AND_tb;
reg aa,bb;
wire dd00,dd11,dd22,dd33;
two_to_four_decoder_AND u_test(
.A (aa),
.B (bb),

.D0 (dd00),
.D1 (dd11),
.D2 (dd22),
.D3 (dd33)
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
