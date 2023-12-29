module Buffer #(
    parameter DATA_WIDTH = 32
)
(
    input clk_i,
    input reset_n_i,

    input[DATA_WIDTH-1:0] data_i,

    output[DATA_WIDTH-1:0] data_o
);

    reg[DATA_WIDTH-1:0] data;

    assign data_o = data;

    always @(posedge clk_i, negedge reset_n_i) begin
        if(reset_n_i == 1'b0) begin
            data <= {DATA_WIDTH{1'b0}};
        end
        else begin
            data <= data_i;
        end
    end
endmodule

