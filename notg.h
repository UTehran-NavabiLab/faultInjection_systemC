#include "systemc.h"
#include <string>
#include <iostream>
#include "faultInjection_package.h.h"
using namespace std;

class notg : public SC_MODULE_FAULTABLE {
protected:
	int hardwareObjectId;
	faultRegistry* accessRegistry;
	
public:
	sc_in <sc_logic> in1;
	sc_out <sc_logic> out1;
	
	faultProperty faults[2];

	SC_HAS_PROCESS(notg);
	notg(sc_module_name _name, faultRegistry* accessRegistryIn){
		// Register itself and gets its unique ID
		accessRegistry = accessRegistryIn;
		hardwareObjectId = accessRegistry->registerModule(this);
		
		// Define faults
		faults[0].setFaultProperty(hardwareObjectId,1,1,SA0); //objId:1 for in1
		faults[1].setFaultProperty(hardwareObjectId,1,2,SA1);
		
		// Register faults
		accessRegistry->registerFault(&faults[0]); 
		accessRegistry->registerFault(&faults[1]);

		SC_METHOD(prc_Original_notg);
		sensitive << in1 << faultInjected;
	}

	// Incorporate faults in the functionality
	void prc_Original_notg(){
		
		cout << "fault on in1 = " << accessRegistry->getObjectFaultType(hardwareObjectId,1) <<  " --- Time: " << sc_time_stamp() << endl;
			
		if (accessRegistry->getObjectFaultType(hardwareObjectId,1) == NoFault){
			if (in1->read() == SC_LOGIC_1){
				out1->write(SC_LOGIC_0);
			} else if (in1->read() == SC_LOGIC_0){
				out1->write(SC_LOGIC_1);
			}
		}
		else{	
			if (accessRegistry->getObjectFaultType(hardwareObjectId,1) == SA0)
				out1 = SC_LOGIC_1;
			
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,1) == SA1)
				out1 = SC_LOGIC_0;
			else
				out1 = SC_LOGIC_X;
		}
	}
};