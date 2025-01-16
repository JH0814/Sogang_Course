
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/12/02 11:04:10
// Design Name: 
// Module Name: Up_Down_Counter
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
module Up_Down_Counter(
input clk, rst, flag,
output[3:0] out,
output[7:0] DS,
output sett
    );
    reg[3:0] out;
    reg[7:0] DS;
    assign sett = 1;
    always@(posedge clk) begin
    if(rst == 1'b1)begin
        if(flag == 1'b1) begin
            out = 4'b0000;
            DS = 8'b00111110;
        end
        else begin
            out = 4'b1111;
            DS = 8'b00111101;
        end
    end
    else begin
        if(flag == 1'b1) begin
            out = out + 1;
            DS = 8'b00111110;
        end
        else begin
            out = out - 1;
            DS = 8'b00111101;
        end
    end
    end
endmodule
