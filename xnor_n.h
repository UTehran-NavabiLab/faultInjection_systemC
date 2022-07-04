#include "systemc.h"
#include <string>
#include <iostream>
#include "FIM.h"
using namespace std;

class xnor_n : public SC_MODULE_FAULTABLE {
protected:
	string hardwareObjectId;
	faultRegistry* accessRegistry;
	
public:
	sc_in <sc_logic> in1[2];
	sc_out <sc_logic> out1;
	
	faultProperty faults[4];

	SC_HAS_PROCESS(xnor_n);
	xnor_n(sc_module_name _name, faultRegistry* accessRegistryIn){
		// Register itself and gets its unique ID
		accessRegistry = accessRegistryIn;
		accessRegistry->registerModule(this);
        hardwareObjectId = _name
		
		// Define faults
		faults[0].setFaultProperty(hardwareObjectId,"in1(0)",1,SA0); //objId:1 for in1[0]
		faults[1].setFaultProperty(hardwareObjectId,"in1(0)",2,SA1);
		faults[2].setFaultProperty(hardwareObjectId,"in1(1)",3,SA0); //objId:2 for in1[1]
		faults[3].setFaultProperty(hardwareObjectId,"in1(1)",4,SA1);
		
		// Register faults
		accessRegistry->registerFault(&faults[0]); 
		accessRegistry->registerFault(&faults[1]);
		accessRegistry->registerFault(&faults[2]);
		accessRegistry->registerFault(&faults[3]);

		SC_METHOD(prc_Original_xnor_n);
		sensitive << in1[0] << in1[1] << faultInjected;
	}

	// Incorporate faults in1 the functionality
	void prc_Original_xnor_n(){
		
		cout << "fault on in1[0] = " << accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") << " --- fault on in1[1] = " 
			<< accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") <<  " --- Time: " << sc_time_stamp() << endl;
			
		if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == NoFault 
			&& accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == NoFault){
			if (in1[0]->read() == in1[1]->read()){
				out1->write(SC_LOGIC_0);
			} else {
				out1->write(SC_LOGIC_1);
			}
		}
		else{	
			if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == SA1 && accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == SA1)
				out1 = SC_LOGIC_1;
			
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == SA0 
					&& accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == SA0)
				out1 = SC_LOGIC_1;
				
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == SA0){
				if (in1[1]->read() == SC_LOGIC_0)
					out1 = SC_LOGIC_1;
				else
					out1 = SC_LOGIC_0;
			}
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == SA0){
				if (in1[0]->read() == SC_LOGIC_0)
					out1 = SC_LOGIC_1;
				else
					out1 = SC_LOGIC_0;
			}
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == SA1){
				if (in1[1]->read() == SC_LOGIC_1)
					out1 = SC_LOGIC_1;
				else
					out1 = SC_LOGIC_0;
			}
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == SA1){
				if (in1[0]->read() == SC_LOGIC_1)
					out1 = SC_LOGIC_1;
				else
					out1 = SC_LOGIC_0;
			}
			
			else
				out1 = SC_LOGIC_X;
		}
	}
};