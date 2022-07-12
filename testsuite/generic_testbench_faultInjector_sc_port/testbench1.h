#include <iostream>
#include <fstream>
#include <string>

#include "systemc.h"
#include "js2sc.h"
#include "fault_injector.h"

using namespace sc_core;

SC_MODULE(testbench){

    sc_signal< sc_logic > i0, i1, ci, s_gdut, co_gdut, s_fdut, co_fdut, clk;

    fulladder* fdut;
    fulladder* gdut;
    fault_injector* flt_injector;

    faultRegistry* accessRegistry;

	SC_HAS_PROCESS(testbench);
	testbench(sc_module_name _name){
        accessRegistry = new faultRegistry();

        flt_injector = new fault_injector("fault_injector", accessRegistry);
            flt_injector->output_ports(ci);
            flt_injector->output_ports(i1);
            flt_injector->output_ports(i0);
            flt_injector->input_ports(co_gdut);
            flt_injector->input_ports(co_fdut);
            flt_injector->input_ports(s_gdut);
            flt_injector->input_ports(s_fdut);

        gdut = new fulladder("fulladder_gold", accessRegistry);
            gdut->i0(i0);
            gdut->i1(i1);
            gdut->ci(ci);
            gdut->s(s_gdut);
            gdut->co(co_gdut);
        fdut = new fulladder("FA_NET", accessRegistry);
            fdut->i0(i0);
            fdut->i1(i1);
            fdut->ci(ci);
            fdut->s(s_fdut);
            fdut->co(co_fdut);
        

    }
};