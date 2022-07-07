#include "systemc.h"
#include "testbench.h"
#include "gates.h"

int sc_main(int argc, char* argv[]){
        faultRegistry* accessRegistry = new faultRegistry();

    testbench *xor_ao2 = new testbench("testbench", accessRegistry);

    sc_trace_file *vcdfile;
    vcdfile = sc_create_vcd_trace_file("vcdfile");
    sc_trace(vcdfile, xor_ao2->xorin1, "xorin1");
    sc_trace(vcdfile, xor_ao2->xorin2, "xorin2");
    sc_trace(vcdfile, xor_ao2->ao2in1, "ao2in1");
    sc_trace(vcdfile, xor_ao2->ao2in2, "ao2in2");
    sc_trace(vcdfile, xor_ao2->ao2in3, "ao2in3");
    sc_trace(vcdfile, xor_ao2->ao2in4, "ao2in4");
    
    sc_trace(vcdfile, xor_ao2->xorout, "xorout");
    sc_trace(vcdfile, xor_ao2->ao2out, "ao2out");

    sc_start(1000, SC_NS);
    return 0;
}