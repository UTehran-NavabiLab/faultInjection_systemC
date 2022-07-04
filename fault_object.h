#include "systemc.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

#ifndef __FAULT_OBJECT_H__
#define __FAULT_OBJECT_H__

enum Faults { SA0, SA1, BitFlip, NoFault };

///////////////////////////////////// future project /////////////////////////////////////
template <class T, sc_write_policy WRITER_POLICY = SC_ONE_WRITER>
class sc_flt_signal : public sc_core::sc_signal<T, WRITER_POLICY>{
	private:
		Faults fault;
	public:
		sc_flt_signal(Faults fault):fault(fault){

		}

		void activate_fault(Faults fault_type, Faults fault_value) {
			if (fault_type == SA){
				if (fault_value == '0'){
					this->write(SC_LOGIC_0);// THIS IS SIGNAL AND DOES NOT CHANGE VALUE UNLESS SCHEDULE BY KERNEL
				}
			}
		}
};
//////////////////////////////////////////////////////////////////////////

#endif