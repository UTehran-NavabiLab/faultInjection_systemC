#include "systemc.h"
#include "testbench.h"

int sc_main(int argc, char* argv[]){
    testbench* fault_simulation = new testbench("counter_TB_vhd");
    
    sc_trace_file *vcdfile;
    vcdfile = sc_create_vcd_trace_file("vcdfile");
    vcdfile->set_time_unit(1, SC_NS);

    sc_trace(vcdfile, fault_simulation->clk, "clk");
    sc_trace(vcdfile, fault_simulation->Si, "Si");
    sc_trace(vcdfile, fault_simulation->NbarT, "NbarT");
    sc_trace(vcdfile, fault_simulation->fdut->dff_19->Q, "dff_19_Q");
    sc_trace(vcdfile, fault_simulation->fdut->dff_19->Si, "dff_19_Si");
    sc_trace(vcdfile, fault_simulation->fdut->dff_20->Q, "dff_20_Q");
    sc_trace(vcdfile, fault_simulation->fdut->dff_20->Si, "dff_20_Si");
    sc_trace(vcdfile, fault_simulation->fdut->dff_21->Q, "dff_21_Q");
    sc_trace(vcdfile, fault_simulation->fdut->dff_21->Si, "dff_21_Si");
    sc_trace(vcdfile, fault_simulation->fdut->dff_22->Q, "dff_22_Q");
    sc_trace(vcdfile, fault_simulation->fdut->dff_22->Si, "dff_22_Si");

 
    sc_start(50000, SC_NS);
    return 0;
}
