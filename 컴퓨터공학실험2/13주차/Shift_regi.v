
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/12/02 10:31:02
// Design Name: 
// Module Name: Shift_regi
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
module Shift_regi(
input clk, rst, in,
output [3:0] out
    );
    reg [3:0] out;
    initial out = 4'b0000;
    always@(posedge clk) begin
    if(rst == 1'b1) out = 4'b0000;
    else begin
    out = out >> 1;
    out[3] = in;
    end
    end
endmodule
