#include "systemc.h"
#include "testbench.h"

int sc_main(int argc, char* argv[]){
    // creating fault regitry class
    faultRegistry* accessRegistry = new faultRegistry();
    testbench* counter_tb = new testbench("testbench", accessRegistry);
    
    std::cout << "testbench created" << std::endl;

    sc_trace_file *vcdfile;
    vcdfile = sc_create_vcd_trace_file("vcdfile");
    vcdfile->set_time_unit(1, SC_NS);

    sc_trace(vcdfile, counter_tb->rst, "rst");
    sc_trace(vcdfile, counter_tb->clk, "clk");
    sc_trace(vcdfile, counter_tb->en, "en");
    sc_trace(vcdfile, counter_tb->co, "co");
    sc_trace(vcdfile, counter_tb->counter[3], "counter");
    sc_trace(vcdfile, counter_tb->counter[2], "counter");
    sc_trace(vcdfile, counter_tb->counter[1], "counter");
    sc_trace(vcdfile, counter_tb->counter[0], "counter");

    sc_start(1000, SC_NS);
    return 0;
}
