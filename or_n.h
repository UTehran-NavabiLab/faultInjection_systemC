#include "systemc.h"
#include <string>
#include <iostream>
#include "faultInjection_package.h.h"
using namespace std;

class or_n : public SC_MODULE_FAULTABLE {
protected:
	int hardwareObjectId;
	faultRegistry* accessRegistry;
	
public:
	sc_in <sc_logic> in1[2];
	sc_out <sc_logic> out1;
	
	faultProperty faults[4];

	SC_HAS_PROCESS(or_n);
	or_n(sc_module_name _name, faultRegistry* accessRegistryIn){
		// Register itself and gets its unique ID
		accessRegistry = accessRegistryIn;
		hardwareObjectId = accessRegistry->registerModule(this);
		
		// Define faults
		faults[0].setFaultProperty(hardwareObjectId,1,1,SA0); //objId:1 for in1[0]
		faults[1].setFaultProperty(hardwareObjectId,1,2,SA1);
		faults[2].setFaultProperty(hardwareObjectId,2,3,SA0); //objId:2 for in1[1]
		faults[3].setFaultProperty(hardwareObjectId,2,4,SA1);
		
		// Register faults
		accessRegistry->registerFault(&faults[0]); 
		accessRegistry->registerFault(&faults[1]);
		accessRegistry->registerFault(&faults[2]);
		accessRegistry->registerFault(&faults[3]);

		SC_METHOD(prc_Original_or_n);
		sensitive << in1[0] << in1[1] << faultInjected;
	}

	// Incorporate faults in the functionality
	void prc_Original_or_n(){
		
		cout << "fault on in1[0] = " << accessRegistry->getObjectFaultType(hardwareObjectId,1) << " --- fault on in1[1] = " 
			<< accessRegistry->getObjectFaultType(hardwareObjectId,2) <<  " --- Time: " << sc_time_stamp() << endl;
			
		if (accessRegistry->getObjectFaultType(hardwareObjectId,1) == NoFault 
			&& accessRegistry->getObjectFaultType(hardwareObjectId,2) == NoFault){
			out1 = in1[0] | in1[1];
		}
		else{	
			if (accessRegistry->getObjectFaultType(hardwareObjectId,1) == SA1 || accessRegistry->getObjectFaultType(hardwareObjectId,2) == SA1)
				out1 = SC_LOGIC_1;
			
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,1) == SA0 
					&& accessRegistry->getObjectFaultType(hardwareObjectId,2) == SA0)
				out1 = SC_LOGIC_0;
				
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,1) == SA0)
				out1 = SC_LOGIC_0 | in1[1];
			
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,2) == SA0)
				out1 = in1[0] | SC_LOGIC_0;
			
			else
				out1 = SC_LOGIC_X;
		}
	}
};