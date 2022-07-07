#include "systemc.h"
#include "FIM.h"
//SystemC Library
//Navabi Lab Group
//School of ECE, University of Tehran

// TODO: parametrize gates

#ifndef __COMPONENT_FLT_LIB_H__
#define __COMPONENT_FLT_LIB_H__

const vector<string> getModuleName(SC_MODULE_FAULTABLE* module){
	string full_name;
	vector<string> name;

	full_name = module->name();

	// convert string to stringstream to use stream properties (here getline)
	stringstream sstream(full_name);
	string tempString;
	while(getline(sstream, tempString, '.')){
		name.push_back(tempString);
	}


	for(vector<string>::iterator it = name.begin(); it != name.end(); it++){
		std::cout << (*it) << std::endl;
	}
	
	return name;
}

class and_n_flt : public SC_MODULE_FAULTABLE {
protected:

	faultRegistry* accessRegistry;
	
public:
	sc_in <sc_logic> in1[2];
	sc_out <sc_logic> out1;
	
	faultProperty faults[4];

	SC_HAS_PROCESS(and_n_flt);
	and_n_flt(sc_module_name _name, faultRegistry* accessRegistryIn){
		// Register itself and gets its unique ID
		accessRegistry = accessRegistryIn;
		accessRegistry->registerModule(this);
        vector<string> full_name = getModuleName(this);

        // testbenchId = full_name[0];
        // designId = full_name[1];
        // hardwareObjectId = full_name[2];
		testbenchId = "testbenchId";
		designId = "designId";
		hardwareObjectId = "hardwareObjectId";        
		
		// Define faults
		faults[0].setFaultProperty(testbenchId, designId, hardwareObjectId,"in1(0)",1,SA0); //objId:1 for in1[0]
		faults[1].setFaultProperty(testbenchId, designId, hardwareObjectId,"in1(0)",2,SA1);
		faults[2].setFaultProperty(testbenchId, designId, hardwareObjectId,"in1(1)",3,SA0); //objId:2 for in1[1]
		faults[3].setFaultProperty(testbenchId, designId, hardwareObjectId,"in1(1)",4,SA1);
		
		// Register faults
		accessRegistry->registerFault(&faults[0]); 
		accessRegistry->registerFault(&faults[1]);
		accessRegistry->registerFault(&faults[2]);
		accessRegistry->registerFault(&faults[3]);

		SC_METHOD(prc_Original_and_n);
		sensitive << in1[0] << in1[1] << faultInjected;
	}

	// Incorporate faults in the functionality
	void prc_Original_and_n(){
		
			
		if (accessRegistry->getObjectFaultType(testbenchId, designId, hardwareObjectId,"in1(0)") == NoFault 
			&& accessRegistry->getObjectFaultType(testbenchId, designId, hardwareObjectId,"in1(1)") == NoFault){
			out1 = in1[0] & in1[1];
		}
		else{	
			std::cout << "fault on " << hardwareObjectId << " in1[0] = " << accessRegistry->getObjectFaultType(testbenchId, designId, hardwareObjectId,"in1(0)") << " --- fault on in1[1] = " 
				<< accessRegistry->getObjectFaultType(testbenchId, designId, hardwareObjectId,"in1(1)") <<  " --- Time: " << sc_time_stamp() << std::endl;
			
			if (accessRegistry->getObjectFaultType(testbenchId, designId, hardwareObjectId,"in1(0)") == SA0 || accessRegistry->getObjectFaultType(testbenchId, designId, hardwareObjectId,"in1(1)") == SA0)
				out1 = SC_LOGIC_0;
			
			else if (accessRegistry->getObjectFaultType(testbenchId, designId, hardwareObjectId,"in1(0)") == SA1 
					&& accessRegistry->getObjectFaultType(testbenchId, designId, hardwareObjectId,"in1(1)") == SA1)
				out1 = SC_LOGIC_1;
				
			else if (accessRegistry->getObjectFaultType(testbenchId, designId, hardwareObjectId,"in1(0)") == SA1)
				out1 = SC_LOGIC_1 & in1[1];
			
			else if (accessRegistry->getObjectFaultType(testbenchId, designId, hardwareObjectId,"in1(1)") == SA1)
				out1 = in1[0] & SC_LOGIC_1;
			
			else
				out1 = SC_LOGIC_X;
		}
	}
};


#endif


