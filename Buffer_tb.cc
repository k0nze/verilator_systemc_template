#include <systemc.h>
#include <verilated.h>
#include <verilated_vcd_sc.h>

#include "VBuffer.h"

#include <iostream>

int sc_main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    Verilated::traceEverOn(true);

    // get vcd file path from command line arguments
    std::string vcd_file_path;

    if(argc == 2) {
        vcd_file_path = std::string(argv[1]);
    }

    // signals
    sc_clock clk_i{"clk", 1, SC_NS, 0.5, 0, SC_NS, true};
    sc_signal<bool> reset_n_i;

    // input
    sc_signal<uint32_t> data_i;

    // output
    sc_signal<uint32_t> data_o;

    const std::unique_ptr<VBuffer> buffer{new VBuffer{"buffer"}};

    buffer->clk_i(clk_i);
    buffer->reset_n_i(reset_n_i);

    buffer->data_i(data_i);
    buffer->data_o(data_o);

    // start simulation and trace
    std::cout << "VBuffer start!" << std::endl;

    sc_start(0, SC_NS);

    VerilatedVcdSc* trace = new VerilatedVcdSc();
    buffer->trace(trace, 99);

    if(vcd_file_path.empty()) {
        trace->open("VBuffer_tb.vcd");
    } else {
        trace->open(vcd_file_path.c_str());
    }

    // reset
    sc_start(1, SC_NS);
    reset_n_i.write(0);
    sc_start(1, SC_NS);
    reset_n_i.write(1);
    sc_start(1, SC_NS);

    assert(data_o.read() == 0);

    sc_start(1, SC_NS);

    data_i.write(42);
    sc_start(1, SC_NS);

    data_i.write(0);
    sc_start(1, SC_NS);

    // check if output is 42
    assert(data_o.read() == 42);
    sc_start(1, SC_NS);

    // check if output is 0
    assert(data_o.read() == 0);
    sc_start(10, SC_NS);

    buffer->final();

    trace->flush();
    trace->close();

    delete trace;

    std::cout << "VBuffer done!" << std::endl;
    return 0;
}
