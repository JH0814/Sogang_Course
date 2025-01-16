
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/24 13:32:07
// Design Name: 
// Module Name: decade_2421_Counter
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
module decade_2421_Counter(
input x, reset, clk,
output [3:0] res,
output z
    );
    reg [3:0] Q, res;
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
            res[3] <= ~(~Q[3]&~(Q[2]&Q[1])&~(Q[2]&Q[0]));
            res[2] <= ~(~Q[3]&~(Q[2]&~Q[1]&~Q[0])&~(Q[2]&Q[1]));
            res[1] <= ~(~Q[3]&~(~Q[2]&Q[1]) & ~(Q[2]&~Q[1]&Q[0]));
            res[0] <= Q[0];
        end
endmodule
