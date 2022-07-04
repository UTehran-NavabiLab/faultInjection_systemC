#include "systemc.h"
#include "FIM.h"
//SystemC Library
//Navabi Lab Group
//School of ECE, University of Tehran

// TODO: parametrize gates

#ifndef __COMPONENT_FLT_LIB_H__
#define __COMPONENT_FLT_LIB_H__

/////////////////////////////////////////////////////////////////////////////////////
//    Buf
/////////////////////////////////////////////////////////////////////////////////////

class bufg : public SC_MODULE_FAULTABLE {
protected:

	faultRegistry* accessRegistry;
	
public:
	sc_in <sc_logic> in1;
	sc_out <sc_logic> out1;
	
	faultProperty faults[2];

	SC_HAS_PROCESS(bufg);
	bufg(sc_module_name _name, faultRegistry* accessRegistryIn){
		// Register itself and gets its unique ID
		accessRegistry = accessRegistryIn;
		accessRegistry->registerModule(this);
        hardwareObjectId = _name;
		
		// Define faults
		faults[0].setFaultProperty(hardwareObjectId,"in1",1,SA0); //objId:1 for in1
		faults[1].setFaultProperty(hardwareObjectId,"in1",2,SA1);
		
		// Register faults
		accessRegistry->registerFault(&faults[0]); 
		accessRegistry->registerFault(&faults[1]);

		SC_METHOD(prc_Original_bufg);
		sensitive << in1 << faultInjected;
	}

	// Incorporate faults in the functionality
	void prc_Original_bufg(){
		
		cout << "fault on " << hardwareObjectId << " in1 = "  << accessRegistry->getObjectFaultType(hardwareObjectId,"in1") <<  " --- Time: " << sc_time_stamp() << endl;
			
		if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1") == NoFault){
			out1 = in1;
		}
		else{	
			if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1") == SA0)
				out1 = SC_LOGIC_0;
			
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1") == SA1)
				out1 = SC_LOGIC_1;
			else
				out1 = SC_LOGIC_X;
		}
	}
};

/////////////////////////////////////////////////////////////////////////////////////
//    Not
/////////////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////////
//    And
/////////////////////////////////////////////////////////////////////////////////////

class and_n : public SC_MODULE_FAULTABLE {
protected:

	faultRegistry* accessRegistry;
	
public:
	sc_in <sc_logic> in1[2];
	sc_out <sc_logic> out1;
	
	faultProperty faults[4];

	SC_HAS_PROCESS(and_n);
	and_n(sc_module_name _name, faultRegistry* accessRegistryIn){
		// Register itself and gets its unique ID
		accessRegistry = accessRegistryIn;
		accessRegistry->registerModule(this);
        hardwareObjectId = _name;
		
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

		SC_METHOD(prc_Original_and_n);
		sensitive << in1[0] << in1[1] << faultInjected;
	}

	// Incorporate faults in the functionality
	void prc_Original_and_n(){
		
		cout << "fault on " << hardwareObjectId << " in1[0] = " << accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") << " --- fault on in1[1] = " 
			<< accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") <<  " --- Time: " << sc_time_stamp() << endl;
			
		if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == NoFault 
			&& accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == NoFault){
			out1 = in1[0] & in1[1];
		}
		else{	
			if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == SA0 || accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == SA0)
				out1 = SC_LOGIC_0;
			
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == SA1 
					&& accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == SA1)
				out1 = SC_LOGIC_1;
				
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == SA1)
				out1 = SC_LOGIC_1 & in1[1];
			
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == SA1)
				out1 = in1[0] & SC_LOGIC_1;
			
			else
				out1 = SC_LOGIC_X;
		}
	}
};


/////////////////////////////////////////////////////////////////////////////////////
//    Or
/////////////////////////////////////////////////////////////////////////////////////

class or_n : public SC_MODULE_FAULTABLE {
protected:

	faultRegistry* accessRegistry;
	
public:
	sc_in <sc_logic> in1[2];
	sc_out <sc_logic> out1;
	
	faultProperty faults[4];

	SC_HAS_PROCESS(or_n);
	or_n(sc_module_name _name, faultRegistry* accessRegistryIn){
		// Register itself and gets its unique ID
		accessRegistry = accessRegistryIn;
		accessRegistry->registerModule(this);
        hardwareObjectId = _name;
		
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

		SC_METHOD(prc_Original_or_n);
		sensitive << in1[0] << in1[1] << faultInjected;
	}

	// Incorporate faults in the functionality
	void prc_Original_or_n(){
		
		cout << "fault on " << hardwareObjectId << " in1[0] = " << accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") << " --- fault on in1[1] = " 
			<< accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") <<  " --- Time: " << sc_time_stamp() << endl;
			
		if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == NoFault 
			&& accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == NoFault){
			out1 = in1[0] | in1[1];
		}
		else{	
			if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == SA1 || accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == SA1)
				out1 = SC_LOGIC_1;
			
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == SA0 
					&& accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == SA0)
				out1 = SC_LOGIC_0;
				
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == SA0)
				out1 = SC_LOGIC_0 | in1[1];
			
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == SA0)
				out1 = in1[0] | SC_LOGIC_0;
			
			else
				out1 = SC_LOGIC_X;
		}
	}
};



/////////////////////////////////////////////////////////////////////////////////////
//    Nand
/////////////////////////////////////////////////////////////////////////////////////

class nand_n : public SC_MODULE_FAULTABLE {
protected:

	faultRegistry* accessRegistry;
	
public:
	sc_in <sc_logic> in1[2];
	sc_out <sc_logic> out1;
	
	faultProperty faults[4];

	SC_HAS_PROCESS(nand_n);
	nand_n(sc_module_name _name, faultRegistry* accessRegistryIn){
		// Register itself and gets its unique ID
		accessRegistry = accessRegistryIn;
		accessRegistry->registerModule(this);
        hardwareObjectId = _name;
		
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

		SC_METHOD(prc_Original_nand_n);
		sensitive << in1[0] << in1[1] << faultInjected;
	}

	// Incorporate faults in the functionality
	void prc_Original_nand_n(){
		
		cout << "fault on " << hardwareObjectId << " in1[0] = " << accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") << " --- fault on in1[1] = " 
			<< accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") <<  " --- Time: " << sc_time_stamp() << endl;
			
		if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == NoFault 
			&& accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == NoFault){
			if ((in1[0]->read() == SC_LOGIC_1) && (in1[1]->read() == SC_LOGIC_1)){
				out1->write(SC_LOGIC_0);
			} else {
				out1->write(SC_LOGIC_1);
			}
		}
		else{	
			if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == SA0 || accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == SA0)
				out1 = SC_LOGIC_1;
			
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == SA1 
					&& accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == SA1)
				out1 = SC_LOGIC_0;
				
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == SA1){

				if (in1[1]->read() == SC_LOGIC_1)
					out1 = SC_LOGIC_0;
				else
					out1 = SC_LOGIC_1;
			}
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == SA1){
				if (in1[0]->read() == SC_LOGIC_1)
					out1 = SC_LOGIC_0;
				else
					out1 = SC_LOGIC_1;
			}
			else
				out1 = SC_LOGIC_X;
		}
	}
};

/////////////////////////////////////////////////////////////////////////////////////
//    Nor
/////////////////////////////////////////////////////////////////////////////////////

class nor_n : public SC_MODULE_FAULTABLE {
protected:

	faultRegistry* accessRegistry;
	
public:
	sc_in <sc_logic> in1[2];
	sc_out <sc_logic> out1;
	
	faultProperty faults[4];

	SC_HAS_PROCESS(nor_n);
	nor_n(sc_module_name _name, faultRegistry* accessRegistryIn){
		// Register itself and gets its unique ID
		accessRegistry = accessRegistryIn;
		accessRegistry->registerModule(this);
        hardwareObjectId = _name;
		
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

		SC_METHOD(prc_Original_nor_n);
		sensitive << in1[0] << in1[1] << faultInjected;
	}

	// Incorporate faults in the functionality
	void prc_Original_nor_n(){
		
		cout << "fault on " << hardwareObjectId << " in1[0] = " << accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") << " --- fault on in1[1] = " 
			<< accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") <<  " --- Time: " << sc_time_stamp() << endl;
			
		if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == NoFault 
			&& accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == NoFault){
			if ((in1[0]->read() == SC_LOGIC_0) && (in1[1]->read() == SC_LOGIC_0)){
				out1->write(SC_LOGIC_1);
			} else {
				out1->write(SC_LOGIC_0);
			}
		}
		else{	
			if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == SA1 || accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == SA1)
				out1 = SC_LOGIC_0;
			
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
			else
				out1 = SC_LOGIC_X;
		}
	}
};


/////////////////////////////////////////////////////////////////////////////////////
//    Xor
/////////////////////////////////////////////////////////////////////////////////////

class xor_n : public SC_MODULE_FAULTABLE {
protected:

	faultRegistry* accessRegistry;
	
public:
	sc_in <sc_logic> in1[2];
	sc_out <sc_logic> out1;
	
	faultProperty faults[4];

	SC_HAS_PROCESS(xor_n);
	xor_n(sc_module_name _name, faultRegistry* accessRegistryIn){
		// Register itself and gets its unique ID
		accessRegistry = accessRegistryIn;
		accessRegistry->registerModule(this);
        hardwareObjectId = _name;
		
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

		SC_METHOD(prc_Original_xor_n);
		sensitive << in1[0] << in1[1] << faultInjected;
	}

	// Incorporate faults in1 the functionality
	void prc_Original_xor_n(){
		
		cout << "fault on " << hardwareObjectId << " in1[0] = " << accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") << " --- fault on in1[1] = " 
			<< accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") <<  " --- Time: " << sc_time_stamp() << endl;
			
		if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == NoFault 
			&& accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == NoFault){
			if (in1[0]->read() == in1[1]->read()){
				out1->write(SC_LOGIC_1);
			} else {
				out1->write(SC_LOGIC_0);
			}
		}
		else{	
			if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == SA1 && accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == SA1)
				out1 = SC_LOGIC_0;
			
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == SA0 
					&& accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == SA0)
				out1 = SC_LOGIC_0;
				
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == SA0){
				if (in1[1]->read() == SC_LOGIC_0)
					out1 = SC_LOGIC_0;
				else
					out1 = SC_LOGIC_1;
			}
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == SA0){
				if (in1[0]->read() == SC_LOGIC_0)
					out1 = SC_LOGIC_0;
				else
					out1 = SC_LOGIC_1;
			}
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") == SA1){
				if (in1[1]->read() == SC_LOGIC_1)
					out1 = SC_LOGIC_0;
				else
					out1 = SC_LOGIC_1;
			}
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1(1)") == SA1){
				if (in1[0]->read() == SC_LOGIC_1)
					out1 = SC_LOGIC_0;
				else
					out1 = SC_LOGIC_1;
			}
			
			else
				out1 = SC_LOGIC_X;
		}
	}
};


/////////////////////////////////////////////////////////////////////////////////////
//    Xnor
/////////////////////////////////////////////////////////////////////////////////////

class xnor_n : public SC_MODULE_FAULTABLE {
protected:

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
        hardwareObjectId = _name;
		
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
		
		cout << "fault on " << hardwareObjectId << " in1[0] = " << accessRegistry->getObjectFaultType(hardwareObjectId,"in1(0)") << " --- fault on in1[1] = " 
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


/////////////////////////////////////////////////////////////////////////////////////
//    Primary Input      
/////////////////////////////////////////////////////////////////////////////////////

class pin : public SC_MODULE_FAULTABLE {
protected:

	faultRegistry* accessRegistry;
	
public:
	sc_in <sc_logic> in1;
	sc_out <sc_logic> out1;
	
	faultProperty faults[2];

	SC_HAS_PROCESS(pin);
	pin(sc_module_name _name, faultRegistry* accessRegistryIn){
		// Register itself and gets its unique ID
		accessRegistry = accessRegistryIn;
		accessRegistry->registerModule(this);
        hardwareObjectId = _name;
		
		// Define faults
		faults[0].setFaultProperty(hardwareObjectId,"in1",1,SA0); //objId:1 for in1
		faults[1].setFaultProperty(hardwareObjectId,"in1",2,SA1);
		
		// Register faults
		accessRegistry->registerFault(&faults[0]); 
		accessRegistry->registerFault(&faults[1]);

		SC_METHOD(prc_Original_pin);
		sensitive << in1 << faultInjected;
	}

	// Incorporate faults in the functionality
	void prc_Original_pin(){
		
		cout << "fault on " << hardwareObjectId << " in1 = "  << accessRegistry->getObjectFaultType(hardwareObjectId,"in1") <<  " --- Time: " << sc_time_stamp() << endl;
			
		if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1") == NoFault){
			out1 = in1;
		}
		else{	
			if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1") == SA0)
				out1 = SC_LOGIC_0;
			
			else if (accessRegistry->getObjectFaultType(hardwareObjectId,"in1") == SA1)
				out1 = SC_LOGIC_1;
			else
				out1 = SC_LOGIC_X;
		}
	}
};


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


/////////////////////////////////////////////////////////////////////////////////////
//    D Flip Flop 
/////////////////////////////////////////////////////////////////////////////////////

class dff : public SC_MODULE_FAULTABLE {
protected:

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
		accessRegistry->registerModule(this);
        hardwareObjectId = _name;
		
		// Define faults
		faults[0].setFaultProperty(hardwareObjectId,"D",1,SA0); //objId:1 for D
		faults[1].setFaultProperty(hardwareObjectId,"D",2,SA1);
		
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
		
		cout << "fault on " << hardwareObjectId << " D = " << accessRegistry->getObjectFaultType(hardwareObjectId,"D") << " --- Time: " << sc_time_stamp() << endl;
		
        if ((C->read() == SC_LOGIC_1) && ((PRE->read() == SC_LOGIC_0) && (CLR->read() == SC_LOGIC_0 && global_reset->read() == SC_LOGIC_0))){
            if (NbarT->read() == SC_LOGIC_1) 
				val.write(Si->read());
            else if (CE->read() == SC_LOGIC_1){
				if (accessRegistry->getObjectFaultType(hardwareObjectId,"D") == NoFault)
					val.write(D->read());
				else{
					if (accessRegistry->getObjectFaultType(hardwareObjectId,"D") == SA0)
						val.write(SC_LOGIC_0);
					else if (accessRegistry->getObjectFaultType(hardwareObjectId,"D") == SA1)
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

#endif


