#include "systemc.h"
#include <iostream>
#include <fstream>
#include <string>
#include "FIM.h"


using namespace sc_core;

SC_MODULE(fault_injector){

    sc_export<sc_signal<sc_logic>> i0_exp, i1_exp, ci_exp, s_gdut_exp, co_gdut_exp, s_fdut_exp, co_fdut_exp;
    sc_signal<sc_logic> i0, i1, ci, s_gdut, co_gdut, s_fdut, co_fdut;
    // sc_out< sc_logic > i0, i1, ci;

    faultRegistry* accessRegistry;

    // SC_CTOR(fault_injector){
	SC_HAS_PROCESS(fault_injector);
	fault_injector(sc_module_name _name, faultRegistry* accessRegistryIn){
        accessRegistry = accessRegistryIn;
        accessRegistry->log_file.open("flt.log");
        
        i0_exp(i0);
        i1_exp(i1);
        ci_exp(ci);
        s_gdut_exp(s_gdut);
        co_gdut_exp(co_gdut);
        s_fdut_exp(s_fdut);
        co_fdut_exp(co_fdut);

        SC_THREAD(faultInjection);
    }

    void faultInjection(void){
        // Variable Definition ----------------------
        bool detected = false;
        int numOfDetecteds = 0;
        int numOfFaults = 0;
        float coverage;
        
        ifstream faultFile;
        ifstream testFile;
        ofstream reportFile;

        vector<vector<string>> flist;
        vector<string> testVectors;
        
        sc_lv<3> testData;

        //--- File Handling ----------------------
        faultFile.open("fault_list.flt");
        flist = accessRegistry->readFaultList(faultFile);
        faultFile.close();

        testFile.open("test_list.txt");      
        while(testFile){
            string line;

            getline(testFile, line);

            if(!testFile){
                break;
            }
            
            testVectors.push_back(line);
        }
        testFile.close();

        reportFile.open("reportFile.txt");
        
        accessRegistry->log_file << "* Initially Disable all faults " << std::endl;
        accessRegistry->removeFaultList(flist, 0);
        accessRegistry->infFaults();

        //--- Outer loop to inject fault ----------------------
        for(int i=1; i <= flist.size(); i++){
            accessRegistry->log_file << "--------- Inject Fault:--------------------- NUMBER:" << i << std::endl;
            accessRegistry->injectFaultList(flist, i);
            numOfFaults++;

            //--- report ----------------------
            reportFile << "TB---->faultNum = " << numOfFaults << " is injected @ " << sc_core::sc_time_stamp();
            detected = false;
            for(int j=0; ((j < testVectors.size()) && (!detected)); j++){
                testData = str2logic<3>(testVectors[j]);
                accessRegistry->log_file << "\n" << "---+++---+++---+++---+++" << std::endl;
                accessRegistry->log_file << "Apply Test Vector: " << testData << std::endl << std::endl;
                
                // Apply Test Vector to signals
                ci.write(testData[0]);
                i1.write(testData[1]);
                i0.write(testData[2]);

                wait(2, SC_NS);
                accessRegistry->log_file << "++ Simulation result for faulty vs gold model: " << std::endl;
                accessRegistry->log_file << "co_gold vs co_faulty: " << std::endl;
                accessRegistry->log_file << co_gdut.read() << "  vs  " << co_fdut.read() << std::endl;
                accessRegistry->log_file << "s_gold vs s_faulty: " << std::endl;
                accessRegistry->log_file << s_gdut.read() << "  vs  " << s_fdut.read() << std::endl;
                accessRegistry->log_file << std::endl;

                //--- if outputs are not matched 
                if(!((co_fdut.read() == co_gdut.read()) && (s_fdut.read() == s_gdut.read()))){
                    detected = true;
                    accessRegistry->log_file << "******* Fault has been detected ********" << std::endl;
                    accessRegistry->log_file << std::endl;
                    //--- write report ----------------------
                    reportFile << ", detected by testVector = " << testData << " @ " << sc_core::sc_time_stamp();
                } //--- endif: test detected the fault
                reportFile << std::endl;
            }//--- endfor: testvectors
            if(detected)
                numOfDetecteds++;
            
            accessRegistry->removeFaultList(flist, i); 
            accessRegistry->log_file << "--------- Remove Fault:--------------------- NUMBER:" << i << std::endl << std::endl;

        }//--- endfor: faultlist


        //--- calculate coverage ----------------------
        coverage = (numOfDetecteds / numOfFaults);

        //--- write report ----------------------
        reportFile << "numOfDetecteds: " << numOfDetecteds << std::endl;
        reportFile << "numOfFaults: " << numOfFaults << std::endl;
        reportFile << "coverage: " << coverage << std::endl;

        //--- close report file ----------------------
        reportFile.close();

        accessRegistry->log_file << "+ End of faultInjection::---------------------------  " << std::endl;
        accessRegistry->log_file.close();
    }
};