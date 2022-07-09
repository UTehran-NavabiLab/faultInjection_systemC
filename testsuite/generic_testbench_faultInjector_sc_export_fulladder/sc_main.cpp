#include "systemc.h"
#include "testbench.h"

int sc_main(int argc, char* argv[]){
    // creating fault regitry class
    testbench* fulladder_tb = new testbench("fulladder_tb");
    
    std::cout << "testbench created" << std::endl;

    sc_trace_file *vcdfile;
    vcdfile = sc_create_vcd_trace_file("vcdfile");
    vcdfile->set_time_unit(1, SC_NS);

    // sc_trace(vcdfile, fulladder_tb->i0, "i0");
    // sc_trace(vcdfile, fulladder_tb->i1, "i1");
    // sc_trace(vcdfile, fulladder_tb->ci, "ci");
    // sc_trace(vcdfile, fulladder_tb->co_fdut, "co_fdut");
    // sc_trace(vcdfile, fulladder_tb->s_fdut, "s_fdut");
    // sc_trace(vcdfile, fulladder_tb->co_gdut, "co_gdut");
    // sc_trace(vcdfile, fulladder_tb->s_gdut, "s_gdut");

    sc_start(1000, SC_NS);
    return 0;
}
