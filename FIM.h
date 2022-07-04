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
	sc_signal <bool> faultInjected;
	SC_MODULE_FAULTABLE() {}
};


class fault_portfolio{
private:
	SC_MODULE_FAULTABLE* faultable_module;
	int base_id;
public:
	fault_portfolio(SC_MODULE_FAULTABLE* faultable_module, int base_id): base_id(base_id){
		// register all ports
	}
	void register_obj(SC_MODULE_FAULTABLE* faultable_module, int base_id){
		for(){
			
		}
	}
};


class faultProperty{
private:
	int moduleId;
	int faultId;
	int objId;
	Faults faultType;
	bool enable;
public:
	faultProperty(){};
	faultProperty(int moduleId, int objId, int faultId, Faults faultType) :moduleId{ moduleId }, objId{ objId }, faultId{ faultId }, faultType{ faultType }, enable(0) {}
	void setFaultProperty(int moduleId, int objId, int faultId, Faults faultType){
		this->moduleId = moduleId; 
		this->objId = objId; 
		this->faultId = faultId; 
		this->faultType = faultType; 
		this->enable = 0;
	}
	int getModuleId(){ return moduleId; }
	int getObjId(){return objId;}
	int getFaultId(){return faultId;}
	Faults getFaultType(){return faultType;}
	bool getEnable(){return enable;}
	void enableFault(){enable=1;}
	void disableFault(){enable=0;}
};


class faultRegistry {
private:
	vector <SC_MODULE_FAULTABLE*> moduleVector;
	vector <int> moduleIdVector;
	vector <faultProperty*> faultVector;
public:
	int registerModule(SC_MODULE_FAULTABLE* regModule);
	void registerFault(faultProperty* regFault);
	void saboteurOn(int moduleId, int objId, int faultId); //search, find, enable
	void saboteurOff(int moduleId, int objId, int faultId); //search, find, disable
	Faults getFaultType(int moduleId, int objId, int faultId); //search, find, getFaultType
	Faults getObjectFaultType(int moduleId, int objId); //search, find, if enabled getFaultType
	void infFaults();
	void disp_faultList(const vector<vector<string>>& fualtList);
	void infStuckAt();
	vector<vector<string>> readFaultList(const fstream& faultList);
	void injectFaultList(const vector<vector<string>>& fualtList, int num_fault);
};

int faultRegistry::registerModule(SC_MODULE_FAULTABLE* regModule) {
	moduleVector.push_back(regModule);
	int moduleId = moduleVector.size()-1;
	moduleIdVector.push_back(moduleId);
	return moduleId;
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

void faultRegistry::saboteurOn(int moduleId, int objId, int faultId){
	for(int i=0; i<faultVector.size(); i++){
		if((faultVector[i]->getModuleId() == moduleId) && (faultVector[i]->getObjId() == objId) && (faultVector[i]->getFaultId() == faultId)){
			faultVector[i]->enableFault();	//Activate fault
	//		int mId = faultVector[i]-> getModuleId();
			moduleVector[moduleId]->faultInjected.write(true); // Inject fault
		}
	}
}

void faultRegistry::saboteurOff(int moduleId, int objId, int faultId){
	for(int i=0; i<faultVector.size(); i++){
		if((faultVector[i]->getModuleId() == moduleId) && (faultVector[i]->getObjId() == objId) && (faultVector[i]->getFaultId() == faultId)){
			faultVector[i]->disableFault();
//			int mId = faultVector[i]-> getModuleId();
			moduleVector[moduleId]->faultInjected.write(false);
		}
	}
}

Faults faultRegistry::getFaultType(int moduleId, int objId, int faultId){
	for(int i=0; i<faultVector.size(); i++){
		if((faultVector[i]->getModuleId() == moduleId) && (faultVector[i]->getObjId() == objId) && (faultVector[i]->getFaultId() == faultId))
			return faultVector[i]->getFaultType();
	}
	return NoFault;
}

Faults faultRegistry::getObjectFaultType(int moduleId, int objId){
	for(int i=0; i<faultVector.size(); i++){
		if((faultVector[i]->getModuleId() == moduleId) && (faultVector[i]->getObjId() == objId) &&(faultVector[i]->getEnable() == true))
			return faultVector[i]->getFaultType();
	}
	return NoFault;
}

vector<vector<string>> faultRegistry::readFaultList(const ifstream& faultList){

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

        complete_flist.push_back(fault_prop);
    }

	return complete_flist;
}

void faultRegistry::injectFaultList(const vector<vector<string>>& fualtList, int num_fault){

}
void faultRegistry::disp_faultList(const vector<vector<string>>& fualtList){
    // the last complete_flist item is dummy
    for(int i = 0; i < fualtList.size() - 1; i++){

        std::cout << "On line " << i << " of fault list there is fault with description below: " << std::endl;

		int j = 0;
		const string prop_name[] = {
			"@ testbench: ",
			"@ design under test: ",
			"@ module name: ",
			"@ port name: "
			};
        for(vector<string>::iterator it = fualtList[i].begin(); it != fualtList[i].end(); it++){
            std::cout << prop_name[j] << (*it) << std::endl;
			j++;
        }
    }
}

#endif