`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/03 14:42:03
// Design Name: 
// Module Name: four_to_two_encoder_tb
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


module four_to_two_encoder_tb;
reg aa,bb, cc, dd;
wire ee00, ee11;
four_to_two_encoder u_test(
.A (aa),
.B (bb),
.C (cc),
.D (dd),

.E0 (ee00),
.E1 (ee11)
);

initial begin
aa = 1'b0;
bb = 1'b0;
cc = 1'b0;
dd = 1'b0;
end

always@(aa or bb or cc or dd) begin
aa <= #50 ~aa;
bb <= #100 ~bb;
cc <= #200 ~cc;
dd <= #400 ~dd;
end
initial begin
#1000
$finish;
end
endmodule
