
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/18 23:43:34
// Design Name: 
// Module Name: D_FF_tb
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
module D_FF_tb;
reg DD, CLK;
wire QQ11, QQ22;
D_FF u_test(
.D (DD),
.clk (CLK),
.Q1 (QQ11),
.Q2 (QQ22)
);
initial begin
CLK = 1'b0;
    forever #10 CLK = ~CLK;
end
initial begin
DD = 1'b0;
#20
DD = 1'b0;
#20
DD = 1'b1;
#20
DD = 1'b0;
#20
DD = 1'b1;
#20
DD = 1'b1;
#20
$finish;
end
endmodule
