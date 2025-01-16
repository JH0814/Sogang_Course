`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/12/02 11:05:10
// Design Name: 
// Module Name: Up_Down_Counter_tb
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


module Up_Down_Counter_tb;
reg clk, rst, flag;
wire[3:0] out;
Up_Down_Counter u_test(
.clk (clk),
.rst (rst),
.flag (flag),
.out (out)
);
initial begin
clk = 1'b0;
rst = 1'b1;
flag = 1'b1;
end
always clk = #10 ~clk;
initial begin
#50
rst = 0;
#350
rst = 1;
flag = 1'b0;
#50
rst = 0;
#350
$finish;
end
endmodule
