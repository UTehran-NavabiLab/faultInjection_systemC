#include "systemc.h"

#ifndef __GATES_H__
#define __GATES_H__

// xor gate
SC_MODULE(xor_gate){

    sc_in< sc_logic > in1, in2;
    sc_out< sc_logic > out;

    SC_CTOR(xor_gate){

        SC_THREAD(eval);
        sensitive << in1 << in2;

    }

    void eval(void);
};

// AO2gates
SC_MODULE(ao2_gate){

    sc_in<sc_logic> in1, in2, in3, in4;
    sc_out<sc_logic> out;

    ao2_gate(sc_module_name);

    void eval(void);
    
};
// full adder


#endif
