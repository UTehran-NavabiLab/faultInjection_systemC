#include "systemc.h"
#include "gates.h"
#include "FIM.h"
#include "utilities.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

SC_MODULE(testbench){

    sc_signal<sc_logic> xorin1, xorin2, ao2in1, ao2in2, ao2in3, ao2in4;
    sc_signal<sc_logic> xorout, ao2out;


    and_n_flt *xor_ptr;
    ao2_gate *ao2;

    faultRegistry* accessRegistry;

    // SC_CTOR(testbench){
	SC_HAS_PROCESS(testbench);
	testbench(sc_module_name _name, faultRegistry* accessRegistryIn){
        accessRegistry = accessRegistryIn;

        SC_THREAD(inputGen);
        SC_THREAD(message);
        SC_THREAD(fault_injection);

        xor_ptr = new and_n_flt("and", accessRegistry);
        (*xor_ptr)(xorin1, xorin2, xorout);

        ao2 = new ao2_gate("AO2");
        (*ao2)(ao2in1, ao2in2, ao2in3, ao2in4, ao2out);

    }


    void fault_injection(void){
        accessRegistry->saboteurOn("testbench", "designId", "moduleId","in1(0)",SA0);
    }

    void message(void){
        // std::cout << "hierarchical naming: " << endl;
        // std::cout << ao2->name() << std::endl;
        // std::cout << "----------" << std::endl;
        // std::cout << ao2->basename() << std::endl;
        // std::cout << "----------" << std::endl;
        // ao2->print();
        // std::cout << "----------" << endl;
        // ao2->dump();
        // std::cout << "hierarchical naming ends :---------- " << endl;
    }
    void inputGen(void){
        // std::cout << "inside inputgen" << endl;
        xorin1.write(SC_LOGIC_0);
        xorin2.write(SC_LOGIC_0);
        ao2in1.write(SC_LOGIC_0);
        ao2in2.write(SC_LOGIC_0);
        ao2in3.write(SC_LOGIC_0);
        ao2in4.write(SC_LOGIC_0);
        // std::cout << xorout;
        // std::cout << ao2out;
        wait(17, SC_NS);
        xorin1.write(SC_LOGIC_1);
        xorin2.write(SC_LOGIC_0);
        ao2in1.write(SC_LOGIC_1);
        ao2in2.write(SC_LOGIC_0);
        ao2in3.write(SC_LOGIC_0);
        ao2in4.write(SC_LOGIC_0);
        // std::cout << xorout;
        // std::cout << ao2out;
        wait(17, SC_NS);
        xorin1.write(SC_LOGIC_0);
        xorin2.write(SC_LOGIC_1);
        ao2in1.write(SC_LOGIC_0);
        ao2in2.write(SC_LOGIC_1);
        ao2in3.write(SC_LOGIC_0);
        ao2in4.write(SC_LOGIC_0);
        // std::cout << xorout;
        // std::cout << ao2out;
        wait(17, SC_NS);
        xorin1.write(SC_LOGIC_1);
        xorin2.write(SC_LOGIC_1);
        ao2in1.write(SC_LOGIC_0);
        ao2in2.write(SC_LOGIC_0);
        ao2in3.write(SC_LOGIC_1);
        ao2in4.write(SC_LOGIC_0);
        // std::cout << xorout;
        // std::cout << ao2out;
        wait(17, SC_NS);
        ao2in1.write(SC_LOGIC_1);
        ao2in2.write(SC_LOGIC_0);
        ao2in3.write(SC_LOGIC_1);
        ao2in4.write(SC_LOGIC_0);
        // std::cout << xorout;
        // std::cout << ao2out;
        wait(17, SC_NS);
        ao2in1.write(SC_LOGIC_0);
        ao2in2.write(SC_LOGIC_1);
        ao2in3.write(SC_LOGIC_1);
        ao2in4.write(SC_LOGIC_1);
        // std::cout << xorout;
        // std::cout << ao2out;
        wait(17, SC_NS);
        ao2in1.write(SC_LOGIC_1);
        ao2in2.write(SC_LOGIC_1);
        ao2in3.write(SC_LOGIC_1);
        ao2in4.write(SC_LOGIC_1);
        // std::cout << xorout;
        // std::cout << ao2out;
        wait(17, SC_NS);
    }


};