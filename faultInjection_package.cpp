#include "systemc.h"
#include <string>
#include <iostream>
using namespace std;

#ifndef __FIM.h_H__
#define __FIM.h_H__

enum Faults { SA0, SA1, BitFlip, NoFault };

SC_MODULE(SC_MODULE_FAULTABLE) {
public:
	sc_signal <bool> faultInjected;
	SC_MODULE_FAULTABLE() {}
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
	void infStuckAt();
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

#endif