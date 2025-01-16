//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/24 13:17:30
// Design Name: 
// Module Name: decade_Counter
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
module decade_Counter(
input x, reset, clk,
output [3:0] Q,
output z
    );
    reg [3:0] Q;
    reg z;
    always@ (posedge clk) begin
        if(reset) begin
            Q = 4'b0000;
            z <= 1'b0;
        end
        else begin
            if(x == 1'b1) begin
                if(Q == 4'b1001) begin
                    z <= 1'b1;
                    Q <= 4'b0000;
                end
                else begin
                    Q <= Q + 1;
                    z <= 1'b0;
                end
            end
        end
        end
endmodule
