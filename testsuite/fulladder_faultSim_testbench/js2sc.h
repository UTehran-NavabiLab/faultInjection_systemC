#include <systemc.h>
#include <string>
#include "component_flt_lib.h"

using namespace sc_core;

SC_MODULE( fulladder ) {

    sc_in<sc_logic> i0;
    sc_in<sc_logic> i1;
    sc_in<sc_logic> ci;
    sc_out<sc_logic> s;
    sc_out<sc_logic> co;

    sc_signal<sc_logic> sc_logic_1_signal;
    sc_signal<sc_logic> sc_logic_0_signal;

    sc_signal<sc_logic> S0;
    sc_signal<sc_logic> S1;
    sc_signal<sc_logic> S2;
    sc_signal<sc_logic> S3;
    sc_signal<sc_logic> S4;
    sc_signal<sc_logic> S5;
    sc_signal<sc_logic> S6;
    sc_signal<sc_logic> S7;
    sc_signal<sc_logic> S8;
    sc_signal<sc_logic> S9;
    sc_signal<sc_logic> S10;
    sc_signal<sc_logic> S11;
    sc_signal<sc_logic> S12;
    sc_signal<sc_logic> S13;
    sc_signal<sc_logic> S14;
    sc_signal<sc_logic> S15;
    sc_signal<sc_logic> S16;

    nor_n_flt* nor_n_0;
    nand_n_flt* nand_n_1;
    nand_n_flt* nand_n_2;
    notg_flt* notg_3;
    nor_n_flt* nor_n_4;
    nand_n_flt* nand_n_5;
    nand_n_flt* nand_n_6;
    nand_n_flt* nand_n_7;
    nand_n_flt* nand_n_8;
    nand_n_flt* nand_n_9;
    nand_n_flt* nand_n_10;
    notg_flt* notg_11;
    notg_flt* notg_12;
    notg_flt* notg_13;
    pin_flt* pin_14;
    pout_flt* pout_15;
    pin_flt* pin_16;
    pin_flt* pin_17;
    pout_flt* pout_18;

SC_HAS_PROCESS(fulladder);
fulladder(sc_module_name _name, faultRegistry* accessRegistry){
        
    nor_n_0 = new nor_n_flt("nor_n_0", accessRegistry);
        nor_n_0->in1[0](S12);
        nor_n_0->in1[1](S15);
        nor_n_0->out1(S11);

    nand_n_1 = new nand_n_flt("nand_n_1", accessRegistry);
        nand_n_1->in1[0](S10);
        nand_n_1->in1[1](S8);
        nand_n_1->out1(S0);

    nand_n_2 = new nand_n_flt("nand_n_2", accessRegistry);
        nand_n_2->in1[0](S12);
        nand_n_2->in1[1](S15);
        nand_n_2->out1(S1);

    notg_3 = new notg_flt("notg_3", accessRegistry);
        notg_3->in1(S1);
        notg_3->out1(S2);

    nor_n_4 = new nor_n_flt("nor_n_4", accessRegistry);
        nor_n_4->in1[0](S2);
        nor_n_4->in1[1](S11);
        nor_n_4->out1(S3);

    nand_n_5 = new nand_n_flt("nand_n_5", accessRegistry);
        nand_n_5->in1[0](S1);
        nand_n_5->in1[1](S0);
        nand_n_5->out1(S4);

    nand_n_6 = new nand_n_flt("nand_n_6", accessRegistry);
        nand_n_6->in1[0](S4);
        nand_n_6->in1[1](S14);
        nand_n_6->out1(S5);

    nand_n_7 = new nand_n_flt("nand_n_7", accessRegistry);
        nand_n_7->in1[0](S3);
        nand_n_7->in1[1](S9);
        nand_n_7->out1(S6);

    nand_n_8 = new nand_n_flt("nand_n_8", accessRegistry);
        nand_n_8->in1[0](S6);
        nand_n_8->in1[1](S5);
        nand_n_8->out1(S16);

    nand_n_9 = new nand_n_flt("nand_n_9", accessRegistry);
        nand_n_9->in1[0](S0);
        nand_n_9->in1[1](S14);
        nand_n_9->out1(S7);

    nand_n_10 = new nand_n_flt("nand_n_10", accessRegistry);
        nand_n_10->in1[0](S7);
        nand_n_10->in1[1](S1);
        nand_n_10->out1(S13);

    notg_11 = new notg_flt("notg_11", accessRegistry);
        notg_11->in1(S15);
        notg_11->out1(S8);

    notg_12 = new notg_flt("notg_12", accessRegistry);
        notg_12->in1(S14);
        notg_12->out1(S9);

    notg_13 = new notg_flt("notg_13", accessRegistry);
        notg_13->in1(S12);
        notg_13->out1(S10);

    pin_14 = new pin_flt("pin_14", accessRegistry);
        pin_14->in1(ci);
        pin_14->out1(S12);

    pout_15 = new pout_flt("pout_15", accessRegistry);
        pout_15->in1(S13);
        pout_15->out1(co);

    pin_16 = new pin_flt("pin_16", accessRegistry);
        pin_16->in1(i0);
        pin_16->out1(S14);

    pin_17 = new pin_flt("pin_17", accessRegistry);
        pin_17->in1(i1);
        pin_17->out1(S15);

    pout_18 = new pout_flt("pout_18", accessRegistry);
        pout_18->in1(S16);
        pout_18->out1(s);


    SC_METHOD(sc_logic_signal_assignment);

}

    void sc_logic_signal_assignment(void){ 
        sc_logic_1_signal.write(SC_LOGIC_1);
        sc_logic_0_signal.write(SC_LOGIC_0);
    }
};
