
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/11 00:07:26
// Design Name: 
// Module Name: BCD_sum_tb
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
module BCD_sum_tb;
reg [3:0]A, B;
reg CIN;
wire [3:0]C1, S;
wire [3:0] SUM;
wire COUT;
BCD_sum u_test(
.a (A),
.b (B),
.cin (CIN),
.s (S),
.c (C1),
.sum (SUM),
.cout (COUT)
);
initial begin
A=4'b0;
B=4'b0;
CIN = 1'b0;
end
always@(A or B or CIN) begin
    CIN <= #1 ~CIN;
    A[0] <= #2 ~A[0];
    A[1] <= #4 ~A[1];
    A[2] <= #8 ~A[2];
    A[3] <= #16 ~A[3];

    B[0] <= #32 ~B[0];
    B[1] <= #64 ~B[1];
    B[2] <= #128 ~B[2];
    B[3] <= #256 ~B[3];
end
initial begin
#1000
$finish;
end
endmodule
