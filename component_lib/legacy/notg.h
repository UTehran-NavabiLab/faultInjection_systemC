#include "systemc.h"
#include <string>
#include <iostream>
#include "FIM.h"
using namespace std;

class notg : public SC_MODULE_FAULTABLE {
protected:

	faultRegistry* accessRegistry;
	
public:
	sc_in <sc_logic> in1;
	sc_out <sc_logic> out1;
	
	faultProperty faults[2];

	SC_HAS_PROCESS(notg);
	notg(sc_module_name _name, faultRegistry* accessRegistryIn){
		// Register itself and gets its unique ID
		// - why would i call registerModule and get a an ID from it
		// 		i'm the lowest level of hierarchy, 
		// 		someone higher in the rank should ask me about myself right after I created as an object
		// - why should I have the access to alllll fault Registry system 
		//		I should only have the access to what's necessary for me, 
		//			like; is the fault applied to my ports, if yes, what is it??
		// - why should I set fault for myself, i'm not to decide somebody else should make that decision
		// - all I care (and use) is getObjectFaultType
		accessRegistry = accessRegistryIn;
		accessRegistry->registerModule(this);
        hardwareObjectId = _name;
		
		// Define faults
		faults[0].setFaultProperty(hardwareObjectId,"in1",1,SA0); //objId:1 for in1
		faults[1].setFaultProperty(hardwareObjectId,"in1",2,SA1);
		
		// Register faults
		accessRegistry->registerFault(&faults[0]); 
		accessRegistry->registerFault(&faults[1]);

		SC_METHOD(prc_Original_notg);
		sensitive << in1 << faultInjected;
	}

	// Incorporate faults in the functionality
	void prc_Original_notg(){
		
		cout << "fault on " << hardwareObjectId << " in1 = "  << accessRegistry->getObjectFaultType(hardwareObjectId,"in1") <<  " --- Time: " << sc_time_stamp() << endl;
			
		if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1") == NoFault){
			if (in1->read() == SC_LOGIC_1){
				out1->write(SC_LOGIC_0);
			} else if (in1->read() == SC_LOGIC_0){
				out1->write(SC_LOGIC_1);
			}
		}
		else{	
			if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1") == SA0)
				out1 = SC_LOGIC_1;
			
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1") == SA1)
				out1 = SC_LOGIC_0;
			else
				out1 = SC_LOGIC_X;
		}
	}
};