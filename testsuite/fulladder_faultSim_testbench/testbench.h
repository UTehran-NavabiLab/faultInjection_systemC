#include "systemc.h"
#include "js2sc.h"
#include <iostream>
#include <fstream>
#include <string>


using namespace sc_core;

SC_MODULE(testbench){

    sc_signal< sc_logic > i0, i1, ci, s_gdut, co_gdut, s_fdut, co_fdut, clk;

    sc_signal<sc_lv<3>> inputs;
    sc_signal<sc_lv<3>> outputs_fdut;
    sc_signal<sc_lv<3>> outputs_gdut;

    fulladder* fdut;
    fulladder* gdut;

    faultRegistry* accessRegistry;

    // SC_CTOR(testbench){
	SC_HAS_PROCESS(testbench);
	testbench(sc_module_name _name, faultRegistry* accessRegistryIn){
        accessRegistry = accessRegistryIn;


        SC_THREAD(faultInjection);
        SC_THREAD(clocking);

        
        gdut = new fulladder("fulladder_gold", accessRegistry);
            gdut->i0(i0);
            gdut->i1(i1);
            gdut->ci(ci);
            gdut->s(s_gdut);
            gdut->co(co_gdut);
        fdut = new fulladder("FA_NET", accessRegistry);
            fdut->i0(i0);
            fdut->i1(i1);
            fdut->ci(ci);
            fdut->s(s_fdut);
            fdut->co(co_fdut);
        

    }

    void clocking(void){
        while(true){
        wait(3, SC_NS);
        clk.write(SC_LOGIC_0);
        wait(3, SC_NS);
        clk.write(SC_LOGIC_1);
        }
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
        
        std::cout << "* Initially Disable all faults " << std::endl;
        accessRegistry->removeFaultList(flist, 0);
        accessRegistry->infFaults();

        //--- Outer loop to inject fault ----------------------
        for(int i=1; i <= flist.size(); i++){
            std::cout << "--------- Inject Fault:--------------------- NUMBER:" << i << std::endl;
            accessRegistry->injectFaultList(flist, i);
            numOfFaults++;

            //--- report ----------------------
            reportFile << "TB---->faultNum = " << numOfFaults << " is injected @ " << sc_core::sc_time_stamp();
            detected = false;
            for(int j=0; ((j < testVectors.size()) && (!detected)); j++){
                testData = str2logic<3>(testVectors[j]);
                std::cout << "\n" << "---+++---+++---+++---+++" << std::endl;
                std::cout << "Apply Test Vector: " << testData << std::endl << std::endl;
                ci.write(testData[0]);
                i1.write(testData[1]);
                i0.write(testData[2]);

                wait(2, SC_NS);
                std::cout << "++ Simulation result for faulty vs gold model: " << std::endl;
                std::cout << "co_gold vs co_faulty: " << std::endl;
                std::cout << co_gdut.read() << "  vs  " << co_fdut.read() << std::endl;
                std::cout << "s_gold vs s_faulty: " << std::endl;
                std::cout << s_gdut.read() << "  vs  " << s_fdut.read() << std::endl;
                std::cout << std::endl;

                //--- if outputs are not matched 
                if(!((co_fdut.read() == co_gdut.read()) && (s_fdut.read() == s_gdut.read()))){
                    detected = true;
                    std::cout << "******* Fault has been detected ********" << std::endl;
                    std::cout << std::endl;
                    //--- write report ----------------------
                    reportFile << ", detected by testVector = " << testData << " @ " << sc_core::sc_time_stamp();
                } //--- endif: test detected the fault
                reportFile << std::endl;
            }//--- endfor: testvectors
            if(detected)
                numOfDetecteds++;
            
            accessRegistry->removeFaultList(flist, i); 
            std::cout << "--------- Remove Fault:--------------------- NUMBER:" << i << std::endl << std::endl;

        }//--- endfor: faultlist


        //--- calculate coverage ----------------------
        coverage = (numOfDetecteds / numOfFaults);

        //--- write report ----------------------
        reportFile << "numOfDetecteds: " << numOfDetecteds << std::endl;
        reportFile << "numOfFaults: " << numOfFaults << std::endl;
        reportFile << "coverage: " << coverage << std::endl;

        //--- close report file ----------------------
        reportFile.close();

        std::cout << "+ End of faultInjection::---------------------------  " << std::endl;
    }
};