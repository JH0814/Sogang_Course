
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/18 11:25:07
// Design Name: 
// Module Name: RS_FF_tb
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
module RS_FF_tb;
reg RR, SS, CLK;
wire QQ11, QQ22;
RS_FF u_test(
.R (RR),
.S (SS),
.clk (CLK),
.Q1 (QQ11),
.Q2 (QQ22)
);
initial begin
CLK = 1'b0;
    forever #10 CLK = ~CLK;
end
initial begin
RR=1'b0;
SS=1'b1;
#20;
RR=1'b0;
SS=1'b0;
#20;
RR=1'b1;
SS=1'b0;
#20;
RR=1'b0;
SS=1'b0;
#20;
RR=1'b1;
SS=1'b0;
#20;
RR=1'b1;
SS=1'b1;
#20;
end
endmodule
