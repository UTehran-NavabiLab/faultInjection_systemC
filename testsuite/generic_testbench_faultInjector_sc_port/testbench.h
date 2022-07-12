#include <iostream>
#include <fstream>
#include <string>
#include "systemc.h"
#include "js2sc.h"
#include "fault_injector.h"

using namespace sc_core;

SC_MODULE( testbench ) {

    sc_signal<sc_logic> i0;
    sc_signal<sc_logic> i1;
    sc_signal<sc_logic> ci;
    sc_signal<sc_logic> s_gld;
    sc_signal<sc_logic> s_flt;
    sc_signal<sc_logic> co_gld;
    sc_signal<sc_logic> co_flt;

    fulladder* flt_dut;
    fulladder* gld_dut;
    fault_injector* flt_injector;
    faultRegistry* accessRegistry;

    SC_HAS_PROCESS(testbench);
    testbench(sc_module_name _name){
        accessRegistry = new faultRegistry();
        flt_injector = new fault_injector("fault_injector", accessRegistry);
            flt_injector->output_ports(ci);
            flt_injector->output_ports(i1);
            flt_injector->output_ports(i0);
            flt_injector->input_ports(s_gld);
            flt_injector->input_ports(s_flt);
            flt_injector->input_ports(co_gld);
            flt_injector->input_ports(co_flt);
        gld_dut = new fulladder("fulladder_gold", accessRegistry);
            gld_dut->i0(i0);
            gld_dut->i1(i1);
            gld_dut->ci(ci);
            gld_dut->s(s_gld);
            gld_dut->co(co_gld);
        flt_dut = new fulladder("FA_NET", accessRegistry);
            flt_dut->i0(i0);
            flt_dut->i1(i1);
            flt_dut->ci(ci);
            flt_dut->s(s_flt);
            flt_dut->co(co_flt);

    }
};
