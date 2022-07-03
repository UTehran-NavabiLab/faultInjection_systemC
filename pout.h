#include "systemc.h"

/////////////////////////////////////////////////////////////////////////////////////
//    Primary Output      
/////////////////////////////////////////////////////////////////////////////////////
class pout : public SC_MODULE_FAULTABLE {
public:
    sc_in< sc_logic > in1; 
    sc_out< sc_logic > out1;

	SC_HAS_PROCESS(pout);
	pout(sc_module_name _name, faultRegistry* accessRegistryIn){
        
        SC_METHOD(eval);
        sensitive << in1;

    }

    void eval(void){
        out1->write(in1->read());
    };
};
