#include <iostream>
#include <fstream>
#include <string>

#include "systemc.h"
#include "js2sc.h"
#include "fault_injector.h"

using namespace sc_core;

SC_MODULE(testbench){

    // sc_signal< sc_logic > i0, i1, ci, s_gdut, co_gdut, s_fdut, co_fdut, clk;

    fulladder* fdut;
    fulladder* gdut;
    fault_injector* flt_injector;

    faultRegistry* accessRegistry;

    // SC_CTOR(testbench){
	SC_HAS_PROCESS(testbench);
	testbench(sc_module_name _name){
        accessRegistry = new faultRegistry();

        flt_injector = new fault_injector("fault_injector", accessRegistry);
            // flt_injector->i0_exp(i0);
            // flt_injector->i1_exp(i1);
            // flt_injector->ci_exp(ci);
            // flt_injector->s_gdut_exp(s_gdut);
            // flt_injector->co_gdut_exp(co_gdut);
            // flt_injector->s_fdut_exp(s_fdut);
            // flt_injector->co_fdut_exp(co_fdut);

        gdut = new fulladder("fulladder_gold", accessRegistry);
            gdut->i0(flt_injector->i0_exp);
            gdut->i1(flt_injector->i1_exp);
            gdut->ci(flt_injector->ci_exp);
            gdut->s(flt_injector->s_gdut_exp);
            gdut->co(flt_injector->co_gdut_exp);
        fdut = new fulladder("FA_NET", accessRegistry);
            fdut->i0(flt_injector->i0_exp);
            fdut->i1(flt_injector->i1_exp);
            fdut->ci(flt_injector->ci_exp);
            fdut->s(flt_injector->s_fdut_exp);
            fdut->co(flt_injector->co_fdut_exp);
        

    }
};