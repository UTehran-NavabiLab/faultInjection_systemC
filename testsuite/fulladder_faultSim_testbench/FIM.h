#include "systemc.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

#ifndef __FIM_H__
#define __FIM_H__

enum Faults { SA0, SA1, BitFlip, NoFault };

SC_MODULE(SC_MODULE_FAULTABLE) {
public:
	string hardwareObjectId;
	sc_signal <bool> faultInjected;
	SC_MODULE_FAULTABLE() {}
};


class faultProperty{
private:
	string moduleId;
	int faultId;
	string objId;
	Faults faultType;
	bool enable;
public:
	faultProperty(){};
	faultProperty(string moduleId, string objId, int faultId, Faults faultType) :moduleId{ moduleId }, objId{ objId }, faultId{ faultId }, faultType{ faultType }, enable(0) {}
	void setFaultProperty(string moduleId, string objId, int faultId, Faults faultType){
		this->moduleId = moduleId; 
		this->objId = objId; 
		this->faultId = faultId; 
		this->faultType = faultType; 
		this->enable = 0;
	}
	string getModuleId(){ return moduleId; }
	string getObjId(){return objId;}
	int getFaultId(){return faultId;}
	Faults getFaultType(){return faultType;}
	bool getEnable(){return enable;}
	void enableFault(){enable=1;}
	void disableFault(){enable=0;}
};


class faultRegistry {
private:
	vector <SC_MODULE_FAULTABLE*> moduleVector;
	vector <string> moduleIdVector;
	vector <faultProperty*> faultVector;
public:
	void registerModule(SC_MODULE_FAULTABLE* regModule);
	void registerFault(faultProperty* regFault);
	void saboteurOn(string moduleId, string objId, Faults faultType); //search, find, enable
	void saboteurOff(string moduleId, string objId, Faults faultType); //search, find, disable
	int getFaultId(string moduleId, string objId, Faults faultType); //search, find, getFaultType
	Faults getObjectFaultType(string moduleId, string objId); //search, find, if enabled getFaultType
	void infFaults();
	void disp_faultList(vector<vector<string>>& faultList);
	void infStuckAt();
	vector<vector<string>> readFaultList(ifstream& faultList);
	void injectFaultList(const vector<vector<string>>& faultList, int faultNumber);
	void removeFaultList(const vector<vector<string>>& faultList, int faultNumber);
};

void faultRegistry::registerModule(SC_MODULE_FAULTABLE* regModule) {
	moduleVector.push_back(regModule);
	string moduleId = regModule->hardwareObjectId;
	moduleIdVector.push_back(moduleId);
	// string moduleId = moduleVector.size()-1;
}

void faultRegistry::registerFault(faultProperty* regFault) {
	faultVector.push_back(regFault);
}

void faultRegistry::infFaults(){
	for(int i=0; i<faultVector.size(); i++)
		cout << "moduleId:" << faultVector[i]->getModuleId() 
		<< " objId:" << faultVector[i]->getObjId() 
		<< " faultId:" << faultVector[i]->getFaultId()
		<< " type:" << faultVector[i]->getFaultType() 
		<< " enable:" << faultVector[i]->getEnable()
		<< endl;
}

void faultRegistry::infStuckAt(){
	for (int i = 0; i < faultVector.size(); i++){
		if (faultVector[i]->getFaultType() == SA0 || faultVector[i]->getFaultType() == SA1){
			cout << "moduleId:" << faultVector[i]->getModuleId()
				<< " objId:" << faultVector[i]->getObjId()
				<< " faultId:" << faultVector[i]->getFaultId()
				<< " type:" << faultVector[i]->getFaultType()
				<< " enable:" << faultVector[i]->getEnable()
				<< endl;
		}
	}
}

void faultRegistry::saboteurOn(string moduleId, string objId, Faults faultType){
	for(int i=0; i<faultVector.size(); i++){
		if((faultVector[i]->getModuleId() == moduleId) && (faultVector[i]->getObjId() == objId) && (faultVector[i]->getFaultType() == faultType)){
			faultVector[i]->enableFault();	//Activate fault
		}
	}
	for (int j=0; j<moduleIdVector.size(); j++){
		if(moduleIdVector[j] == moduleId)
			moduleVector[j]->faultInjected.write(true); // Inject fault
	}
}

void faultRegistry::saboteurOff(string moduleId, string objId, Faults faultType){
	for(int i=0; i<faultVector.size(); i++){
		if((faultVector[i]->getModuleId() == moduleId) && (faultVector[i]->getObjId() == objId) && (faultVector[i]->getFaultType() == faultType)){
			faultVector[i]->disableFault();
		}
	}
	for (int j=0; j<moduleIdVector.size(); j++){
		if(moduleIdVector[j] == moduleId)
			moduleVector[j]->faultInjected.write(false); // deactivate fault
	}	
}

int faultRegistry::getFaultId(string moduleId, string objId, Faults faultType){
	for(int i=0; i<faultVector.size(); i++){
		if((faultVector[i]->getModuleId() == moduleId) && (faultVector[i]->getObjId() == objId) && (faultVector[i]->getFaultType() == faultType))
			return faultVector[i]->getFaultId();
	}
	return NoFault;
}

Faults faultRegistry::getObjectFaultType(string moduleId, string objId){
	for(int i=0; i<faultVector.size(); i++){
		if((faultVector[i]->getModuleId() == moduleId) && (faultVector[i]->getObjId() == objId) &&(faultVector[i]->getEnable() == true))
			return faultVector[i]->getFaultType();
	}
	return NoFault;
}

vector<vector<string>> faultRegistry::readFaultList(ifstream& faultList){

    vector<vector<string>> complete_flist;
    
    while(faultList){
        string line;

        getline(faultList, line);

        // create vector of string to save each token
        vector<string> fault_prop;
        // convert strin to stringstream to use stream properties (here getline)
        stringstream sstream(line);
        string tempString;

        while(getline(sstream, tempString, '/')){
            fault_prop.push_back(tempString);
        }
        
        // on the last round, right before EOF break the loop (otherwise raises seg. fault)
        if(!faultList){
            break;
        }
        // convert "one" char to string
        string str2c(1, fault_prop.back().back());
        fault_prop.push_back(str2c);
        
        // remove two last char (fault value + white space)
        fault_prop[fault_prop.size() - 2].pop_back();
        fault_prop[fault_prop.size() - 2].pop_back();

        complete_flist.push_back(fault_prop);
    }
	return complete_flist;
}

void faultRegistry::injectFaultList(const vector<vector<string>>& faultList, int faultNumber){
	
	// { testbench, dut, module, obj, fault_type }
	string fault_properties[5];

	const string fault_p_title[] = {
		"testbench: ",
		"dut: ",
		"module: ",
		"obj: ",
		"faultType: "
	};

	if(faultNumber <= faultList.size()){
		std::cout << "Fault to be injected: " << std::endl;
		if(faultNumber == 0){ // if (fualtNumber == 0) inject all faults
			// traverse each line (fault)
			for(int i=0; i < faultList.size(); i++){
				// traverse fault properties
				for(int j=0; j < faultList[i].size(); j++){
					fault_properties[j] = faultList[i][j];
					std::cout << fault_p_title[j] << fault_properties[j] << std::endl;
				}

				if(fault_properties[4] == "0")
					this->saboteurOn(fault_properties[2],fault_properties[3],SA0);
				if(fault_properties[4] == "1")
					this->saboteurOn(fault_properties[2],fault_properties[3],SA1);
			}
		} else { // if ( 0 < faultNumber < faultList.size() ) inject one specific fault
			// traverse fault properties
			for(int j=0; j < faultList[faultNumber - 1].size(); j++){
				fault_properties[j] = faultList[faultNumber - 1][j];
				std::cout << fault_p_title[j] << fault_properties[j] << std::endl;
			}

			if(fault_properties[4] == "0")
				this->saboteurOn(fault_properties[2],fault_properties[3],SA0);
			if(fault_properties[4] == "1")
				this->saboteurOn(fault_properties[2],fault_properties[3],SA1);
		}
	} else { // if fault number is greater than fault list range
		std::cout << "OUT OF RANGE ERROR: number exceeds range of fault list" << std::endl;
	}
}

void faultRegistry::removeFaultList(const vector<vector<string>>& faultList, int faultNumber){
	
	// { testbench, dut, module, obj, fault_type }
	string fault_properties[5];

	const string fault_p_title[] = {
		"testbench: ",
		"dut: ",
		"module: ",
		"obj: ",
		"faultType: "
	};

	if(faultNumber <= faultList.size()){
		std::cout << "Fault to be removed: " << std::endl;
		if(faultNumber == 0){ // if (fualtNumber == 0) inject all faults
			// traverse each line (fault)
			for(int i=0; i < faultList.size(); i++){
				// traverse fault properties
				for(int j=0; j < faultList[i].size(); j++){
					fault_properties[j] = faultList[i][j];
					std::cout << fault_p_title[j] << fault_properties[j] << std::endl;
				}

				if(fault_properties[4] == "0")
					this->saboteurOff(fault_properties[2],fault_properties[3],SA0);
				if(fault_properties[4] == "1")
					this->saboteurOff(fault_properties[2],fault_properties[3],SA1);
			}
		} else { // if ( 0 < faultNumber < faultList.size() ) inject one specific fault
			// traverse fault properties
			for(int j=0; j < faultList[faultNumber - 1].size(); j++){
				fault_properties[j] = faultList[faultNumber - 1][j];
				std::cout << fault_p_title[j] << fault_properties[j] << std::endl;
			}

			if(fault_properties[4] == "0")
				this->saboteurOff(fault_properties[2],fault_properties[3],SA0);
			if(fault_properties[4] == "1")
				this->saboteurOff(fault_properties[2],fault_properties[3],SA1);
		}
	} else { // if fault number is greater than fault list range
		std::cout << "OUT OF RANGE ERROR: number exceeds range of fault list" << std::endl;
	}
}

void faultRegistry::disp_faultList(vector<vector<string>>& faultList){
    for(int i = 0; i < faultList.size(); i++){

        std::cout << "On line " << i << " of fault list there is fault with description below: " << std::endl;

		int j = 0;
		const string prop_name[] = {
			"@ testbench: ",
			"@ design under test: ",
			"@ module name: ",
			"@ port name: ",
            "@ stuck at value: "
			};
        for(vector<string>::iterator it = faultList[i].begin(); it != faultList[i].end(); it++){
            std::cout << prop_name[j] << (*it) << std::endl;
			j++;
        }
    }
}

#endif