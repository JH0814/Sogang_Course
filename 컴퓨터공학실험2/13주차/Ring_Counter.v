
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/12/02 10:50:52
// Design Name: 
// Module Name: Ring_Counter
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
module Ring_Counter(
input clk, rst,
output[3:0] out
    );
    reg[3:0] out;
    always@(posedge clk) begin
    if(rst == 1'b1) out = 4'b1000;
    else begin
    out[3] <= out[0];
    out[2] <= out[3];
    out[1] <= out[2];
    out[0] <= out[1];
    end
    end
endmodule
