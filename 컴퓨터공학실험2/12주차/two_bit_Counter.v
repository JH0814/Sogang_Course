
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/24 09:57:05
// Design Name: 
// Module Name: two_bit_Counter
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
module two_bit_Counter(
input clk, reset,x,
output [1:0] Q,
output z
    );
    reg [1:0] Q;
    reg z;
    always@ (posedge clk) begin
    if(reset) begin
        Q[1] <= 1'b0;
        Q[0] <= 1'b0;
        z <= 1'b0;
    end
    else begin
        if(x == 1'b1) begin
            if(Q[1] == 1'b1 && Q[0] == 1'b1) begin
                z <= 1'b1;
                Q <= 2'b00;
            end
            else begin
                Q <= Q + 1;
                z <= 1'b0;
            end
        end
    end
    end
endmodule

