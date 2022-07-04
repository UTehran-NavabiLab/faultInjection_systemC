#include "systemc.h"
#include "js2sc.h"
#include <iostream>

using namespace sc_core;

SC_MODULE(testbench){

    sc_signal< sc_logic > clk, rst, en, co, s1, s0;
    sc_signal< sc_logic > counter[4];

    counter_m* dut;
    faultRegistry* accessRegistry;

    // SC_CTOR(testbench){
	SC_HAS_PROCESS(testbench);
	testbench(sc_module_name _name, faultRegistry* accessRegistryIn){
        accessRegistry = accessRegistryIn;


        std::cout << "testbench:construction begins" << std::endl;
        SC_THREAD(inputGen);
        SC_THREAD(clocking);
        SC_THREAD(faultInjection);

        std::cout << "testbench: counter declaration begins" << std::endl;
        
        dut = new counter_m("counter", accessRegistry);
            dut->clk(clk);
            dut->rst(rst);
            dut->en(en);
            dut->co(co);
            dut->counter[0](counter[0]);
            dut->counter[1](counter[1]);
            dut->counter[2](counter[2]);
            dut->counter[3](counter[3]);
        
        std::cout << "testbench: end of construction" << std::endl;

    }

    void clocking(void){
        // add simulation time restriction like vhd testbench
        std::cout << "clocking" << endl;
        while(true){
            clk.write(SC_LOGIC_1);
            wait(5, SC_NS);
            clk.write(SC_LOGIC_0);
            wait(5, SC_NS);
        }
    }
    void inputGen(void){
        std::cout << "generating input signals" << endl;
        wait(10, SC_NS);
        rst.write(SC_LOGIC_1);
        wait(20, SC_NS);
        rst.write(SC_LOGIC_0);
        en.write(SC_LOGIC_1);
        wait(400, SC_NS);
        en.write(SC_LOGIC_0);
    }

    void faultInjection(void){
        wait(100, SC_NS);
        std::cout << "fault injection begins: " << std::endl;
        accessRegistry->infFaults();
        std::cout << "----------------------------------------------------: " << std::endl;
        //enable stuck at 1 fault on I2
        accessRegistry -> saboteurOn("nand_n_4_inst","in1(0)",2);
        accessRegistry -> saboteurOn("notg_0_inst","in1",1);
        accessRegistry -> saboteurOn("notg_2_inst","in1",2);
        accessRegistry -> saboteurOn("nand_n_3_inst","in(1)",1);
        accessRegistry -> saboteurOn("nor_n_6_inst","in(0)",2);
        accessRegistry -> saboteurOn("dff_19_inst","D",1);
        accessRegistry -> saboteurOn("dff_22_inst","D",1);
        accessRegistry -> saboteurOn("pin_23_inst","in1",2);
        accessRegistry->infFaults();
        std::cout << "fault injection ends: " << std::endl;
        std::cout << "----------------------------------------------------: " << std::endl;
        wait(100, SC_NS);
        std::cout << "fault removal begins: " << std::endl;
        std::cout << "----------------------------------------------------: " << std::endl;
        accessRegistry -> saboteurOff("nand_n_4_inst","in1(0)",2);
        accessRegistry -> saboteurOff("notg_0_inst","in1",1);
        accessRegistry -> saboteurOff("notg_2_inst","in1",2);
        accessRegistry -> saboteurOff("nand_n_3_inst","in(1)",1);
        accessRegistry -> saboteurOff("nor_n_6_inst","in(0)",2);
        accessRegistry -> saboteurOff("dff_19_inst","D",1);
        accessRegistry -> saboteurOff("dff_22_inst","D",1);
        accessRegistry -> saboteurOff("pin_23_inst","in1",2);
        
        accessRegistry->infFaults();
        wait(100, SC_NS);
    }
};