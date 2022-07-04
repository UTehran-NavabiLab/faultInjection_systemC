#include "systemc.h"
#include <string>
#include <iostream>
#include "FIM.h"
using namespace std;

class dff : public SC_MODULE_FAULTABLE {
protected:
	int hardwareObjectId;
	faultRegistry* accessRegistry;
	
public:
    sc_in<sc_logic> D, C, CLR, PRE, CE, NbarT, Si, global_reset;
    sc_out<sc_logic> Q;

    sc_signal<sc_logic, SC_MANY_WRITERS> val;

	faultProperty faults[2];

	SC_HAS_PROCESS(dff);
	dff(sc_module_name _name, faultRegistry* accessRegistryIn){
		// Register itself and gets its unique ID
		accessRegistry = accessRegistryIn;
		hardwareObjectId = accessRegistry->registerModule(this);
		
		// Define faults
		faults[0].setFaultProperty(hardwareObjectId,1,1,SA0); //objId:1 for in1[0]
		faults[1].setFaultProperty(hardwareObjectId,1,2,SA1);
		
		// Register faults
		accessRegistry->registerFault(&faults[0]); 
		accessRegistry->registerFault(&faults[1]);
        
		SC_THREAD(eval);
            sensitive << val;
        SC_METHOD(faultable_set);
            sensitive << C;
        SC_METHOD(reset);
            sensitive << CLR << global_reset;
        SC_METHOD(preset);
            sensitive << PRE;
	}


    void eval(void){
        while(true){
                Q->write(val.read());
            wait();
        }
    }
    
	void faultable_set(void){
		
		cout << "fault on D = " << accessRegistry->getObjectFaultType(hardwareObjectId,1) << " --- Time: " << sc_time_stamp() << endl;
		
        if ((C->read() == SC_LOGIC_1) && ((PRE->read() == SC_LOGIC_0) && (CLR->read() == SC_LOGIC_0 && global_reset->read() == SC_LOGIC_0))){
            if (NbarT->read() == SC_LOGIC_1) 
				val.write(Si->read());
            else if (CE->read() == SC_LOGIC_1){
				if (accessRegistry->getObjectFaultType(hardwareObjectId,1) == NoFault)
					val.write(D->read());
				else{
					if (accessRegistry->getObjectFaultType(hardwareObjectId,1) == SA0)
						val.write(SC_LOGIC_0);
					else if (accessRegistry->getObjectFaultType(hardwareObjectId,1) == SA1)
						val.write(SC_LOGIC_1);
				} 
			// END ELSE IF
        	}
		// END IF 
    	}
	// END FUNCTION: faultable_set
	}

    void reset(void){
        if (CLR->read() == SC_LOGIC_1 || global_reset->read() == SC_LOGIC_1) val.write(SC_LOGIC_0);
    }

    void preset(void){
        if ((PRE->read() == SC_LOGIC_1) && (CLR->read() == SC_LOGIC_0 && global_reset->read() == SC_LOGIC_0)) val.write(SC_LOGIC_1);
    }
};