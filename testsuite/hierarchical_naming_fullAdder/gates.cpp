#include "systemc.h"
#include "gates.h"

// ao2gate constructore
SC_HAS_PROCESS(ao2_gate);
ao2_gate::ao2_gate(sc_module_name _name):sc_module(_name){

    SC_METHOD(eval);
    sensitive << in1 << in2 << in3 << in4;
}

// ao2gate evaluation function
void ao2_gate::eval(void){
    std::cout << "inside ao2 eval" << endl;

	if (((in1->read() & in2->read()) | (in3->read() & in4->read())) == SC_LOGIC_1)
		out->write(SC_LOGIC_1); 
	else
		out->write(SC_LOGIC_0);
}

void xor_gate::eval(void){
    std::cout << "inside xor eval" << endl;

    if (in1->read() == in2->read())
        out->write(SC_LOGIC_0);
    else
        out->write(SC_LOGIC_1);

}