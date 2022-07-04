#include "systemc.h"


/////////////////////////////////////////////////////////////////////////////////////
//    D Flip Flop: DFF_NP0
/////////////////////////////////////////////////////////////////////////////////////
class DFF_NP0 : public SC_MODULE_FAULTABLE {
public:
    sc_in<sc_logic> D, C, R;
    sc_out<sc_logic> Q;

    sc_signal<sc_logic, SC_MANY_WRITERS> val;
    // sc_signal<sc_logic> val;

    // sc_time tphl; 
    // sc_time tplh;

    SC_HAS_PROCESS(DFF_NP0);
	DFF_NP0(sc_module_name _name, faultRegistry* accessRegistryIn){
        : sc_module(_name) {

        SC_THREAD(eval);
            sensitive << val;
        SC_METHOD(set);
            sensitive << C;
        SC_METHOD(reset);
            sensitive << R;
    }

    void eval(void){
        while(true){
                Q->write(val.read());
            wait();
        }
    }
    void set(void){
        if ((C->read() == SC_LOGIC_0) && (R->read() == SC_LOGIC_0)){
            val.write(D->read());
        }
    }

    void reset(void){
        if (R->read() == SC_LOGIC_1) val.write(SC_LOGIC_0);
    }

};

/////////////////////////////////////////////////////////////////////////////////////
//    D Flip Flop: DFF_NP1
/////////////////////////////////////////////////////////////////////////////////////
class DFF_NP1 : public SC_MODULE_FAULTABLE {
public:
    sc_in<sc_logic> D, C, R;
    sc_out<sc_logic> Q;

    sc_signal<sc_logic, SC_MANY_WRITERS> val;
    // sc_signal<sc_logic> val;

    // sc_time tphl; 
    // sc_time tplh;

    SC_HAS_PROCESS(DFF_NP1);
	DFF_NP1(sc_module_name _name, faultRegistry* accessRegistryIn){
    : sc_module(_name) {
    // DFlipFlop(sc_module_name _name, sc_time tphl, sc_time tplh) 
    // : sc_module(_name), tphl(tphl), tplh(tplh) {

        SC_THREAD(eval);
            sensitive << val;
        SC_METHOD(set);
            sensitive << C;
        SC_METHOD(reset);
            sensitive << R;
    }

    void eval(void){
        while(true){
                Q->write(val.read());
            wait();
        }
    }
    void set(void){
        if ((C->read() == SC_LOGIC_0) && (R->read() == SC_LOGIC_0)){
            val.write(D->read());
        }
    }

    void reset(void){
        if (R->read() == SC_LOGIC_1) val.write(SC_LOGIC_1);
    }

};
