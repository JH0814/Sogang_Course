`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/04 11:08:39
// Design Name: 
// Module Name: one_to_four_deMUX_tb
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


module one_to_four_deMUX_tb;
reg aa, bb, FF;
wire AA, BB, CC, DD;
one_to_four_deMUX u_test(
.a (aa),
.b (bb),
.F (FF),

.A (AA),
.B (BB),
.C (CC),
.D (DD)
);
initial begin
aa = 1'b0;
bb = 1'b0;
FF = 1'b0;
end
always@(aa or bb or FF) begin
aa <= #50 ~aa;
bb <= #100 ~bb;
FF <= #200 ~FF;
end
initial begin
#1000
$finish;
end

endmodule
