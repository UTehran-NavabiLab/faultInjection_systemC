#include "systemc.h"
#include "js2sc.h"
#include <iostream>
#include <fstream>

using namespace sc_core;

SC_MODULE(testbench){

    sc_signal< sc_logic > i0, i1, ci, s, co;

    fulladder* dut;
    faultRegistry* accessRegistry;

    // SC_CTOR(testbench){
	SC_HAS_PROCESS(testbench);
	testbench(sc_module_name _name, faultRegistry* accessRegistryIn){
        accessRegistry = accessRegistryIn;


        std::cout << "testbench:construction begins" << std::endl;
        SC_THREAD(inputGen);
        SC_THREAD(faultInjection);

        std::cout << "testbench: counter declaration begins" << std::endl;
        
        dut = new fulladder("fulladder", accessRegistry);
            dut->i0(i0);
            dut->i1(i1);
            dut->ci(ci);
            dut->s(s);
            dut->co(co);
        
        std::cout << "testbench: end of construction" << std::endl;

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
        ifstream fin;
        fin.open("flist.flt");

        vector<vector<string>> flist;
        flist = accessRegistry->readFaultList(fin);

        fin.close();

        wait(100, SC_NS);
        std::cout << "fault injection begins::---------------------------  " << std::endl;
        //enable stuck at 1 fault on I2
        accessRegistry->injectFaultList(flist, 100);

        std::cout << "fault injection ends::--------------------------- " << std::endl;
        std::cout << "fault information::---------------------------  " << std::endl;
        accessRegistry->infFaults();
        std::cout << "----------------------------------------------------: " << std::endl;
        wait(100, SC_NS);
        std::cout << "fault removal begins::---------------------------  " << std::endl;
        accessRegistry->removeFaultList(flist, 100);
        std::cout << "----------------------------------------------------: " << std::endl;
        std::cout << "fault information::---------------------------  " << std::endl;
        accessRegistry->infFaults();
        std::cout << "----------------------------------------------------: " << std::endl;

        
        wait(100, SC_NS);
    }
};