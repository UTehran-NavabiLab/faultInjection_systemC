#include <iostream>
#include <fstream>
#include <string>
#include "systemc.h"
#include "js2sc.h"
#include "fault_injector.h"

using namespace sc_core;

SC_MODULE( testbench ) {

    sc_signal<sc_logic> clk;
    sc_signal<sc_logic> rst;
    sc_signal<sc_logic> en;
    sc_signal<sc_logic> co;
    sc_signal<sc_logic> counter[4];

    // added for scan chain
    sc_signal<sc_logic> NbarT, Si, global_reset;
    sc_signal<sc_logic> So;
    
    // sc_signal<sc_logic> So_flt;
    // sc_signal<sc_logic> co_flt;
    // sc_signal<sc_logic> counter_flt[4];
    // counter_m* flt_dut;
    counter_m* fdut;
    fault_injector<4, 1, 5>* flt_injector;
    faultRegistry* accessRegistry;

    SC_HAS_PROCESS(testbench);
    testbench(sc_module_name _name){
        accessRegistry = new faultRegistry();
        // ********* templated class
        flt_injector = new fault_injector<4, 1, 5>("fault_injector", accessRegistry);
            // output_port[0:2] is always assigned to scan pins
            flt_injector->output_ports(global_reset); // added
            flt_injector->output_ports(NbarT); // added
            flt_injector->output_ports(Si); // added
            
            // output_port[3] is start of actuall design inputs
            flt_injector->output_ports(en);
            
            // input_porta[0:2] is always assigned to scan pins
            flt_injector->input_ports(clk);
            flt_injector->input_ports(rst);
            flt_injector->input_ports(So); // added
            
            // input_port[3] is start of actuall design outputs
            flt_injector->input_ports(co);
            flt_injector->input_ports(counter[0]);
            flt_injector->input_ports(counter[1]);
            flt_injector->input_ports(counter[2]);
            flt_injector->input_ports(counter[3]);

            // flt_injector->input_ports(So_flt); // added
            // flt_injector->input_ports(co_flt);
            // flt_injector->input_ports(counter_flt[0]);
            // flt_injector->input_ports(counter_flt[1]);
            // flt_injector->input_ports(counter_flt[2]);
            // flt_injector->input_ports(counter_flt[3]);
        fdut = new counter_m("FUT", accessRegistry);
            fdut->clk(clk);
            fdut->rst(rst);
            fdut->en(en);
            fdut->co(co);
            fdut->global_reset(global_reset); // added
            fdut->NbarT(NbarT); // added
            fdut->Si(Si); // added
            fdut->So(So); // added
            fdut->counter[0](counter[0]);
            fdut->counter[1](counter[1]);
            fdut->counter[2](counter[2]);
            fdut->counter[3](counter[3]);
        // flt_dut = new counter_m("counter_faulty", accessRegistry);
        //     flt_dut->clk(clk);
        //     flt_dut->rst(rst);
        //     flt_dut->en(en);
        //     flt_dut->co(co_flt);
        //     flt_dut->global_reset(global_reset); // added
        //     flt_dut->NbarT(NbarT); // added
        //     flt_dut->Si(Si); // added
        //     flt_dut->So(So_flt); // added
        //     flt_dut->counter(counter_flt[0]);
        //     flt_dut->counter(counter_flt[1]);
        //     flt_dut->counter(counter_flt[2]);
        //     flt_dut->counter(counter_flt[3]);

        SC_THREAD(clocking); // ****

    }
    
    void clocking(void){
        rst.write(SC_LOGIC_0);
        while(true){
            clk.write(SC_LOGIC_0);
            wait(10, SC_NS);
            clk.write(SC_LOGIC_1);
            wait(10, SC_NS);
        }
    }
};
