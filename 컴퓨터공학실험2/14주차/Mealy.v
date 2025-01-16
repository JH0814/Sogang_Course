
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/12/09 00:25:56
// Design Name: 
// Module Name: Mealy
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
module Mealy(
input clk, rst, in,
output[3:0] out,
output z
    );
    reg[3:0] out;
    reg z;
    initial out = 4'b0000;
    initial z = 1'b0;
    always@(posedge clk) begin
    if(rst == 1'b1) begin
        out = 4'b0000;
        z = 1'b0;
    end
    else begin
        out = out << 1;
        out[0] = in;
        if(in == 1 && out[1] == 0 && out[2] == 1 && out[3] == 1) 
            z = 1'b1;
        else
            z = 1'b0;
    end
    end
endmodule
