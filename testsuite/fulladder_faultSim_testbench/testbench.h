#include "systemc.h"
#include "js2sc.h"
#include "fulladder_gold.h"
#include "utilities.h"
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
    fulladder_gold* gdut;

    faultRegistry* accessRegistry;

    // SC_CTOR(testbench){
	SC_HAS_PROCESS(testbench);
	testbench(sc_module_name _name, faultRegistry* accessRegistryIn){
        accessRegistry = accessRegistryIn;


        std::cout << "testbench:construction begins" << std::endl;
        SC_THREAD(faultInjection);
        SC_THREAD(clocking);

        std::cout << "testbench: counter declaration begins" << std::endl;
        
        gdut = new fulladder_gold("fulladder_gold");
            gdut->i0(i0);
            gdut->i1(i1);
            gdut->ci(ci);
            gdut->s(s_gdut);
            gdut->co(co_gdut);
        fdut = new fulladder("fulladder_faultable", accessRegistry);
            fdut->i0(i0);
            fdut->i1(i1);
            fdut->ci(ci);
            fdut->s(s_fdut);
            fdut->co(co_fdut);
        
        std::cout << "testbench: end of construction" << std::endl;

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

        //--- Outer loop to inject file ----------------------
        for(int i=1; i <= flist.size(); i++){
            std::cout << "inject fault:--------------------- NUMBER:" << i << std::endl;
            accessRegistry->injectFaultList(flist, i);
            numOfFaults++;

            //--- report ----------------------
            reportFile << "TB---->faultNum = " << numOfFaults << " is injected @ " << sc_core::sc_time_stamp();
            detected = false;
            for(int j=0; ((j < testVectors.size()) && (!detected)); j++){
                std::cout << "input test:--------------------- " << std::endl;
                testData = str2logic<3>(testVectors[j]);
                std::cout << "testData: " << testData << std::endl;
                ci.write(testData[0]);
                i1.write(testData[1]);
                i0.write(testData[2]);

                wait(2, SC_NS);
                std::cout << "co_fdut: " << co_fdut.read() << ", co_gdut: " << co_gdut.read() << ", s_fdut: " << s_fdut.read() << ", s_gdut: " << s_gdut.read() << std::endl;

                //--- if outputs are not matched 
                if(!((co_fdut.read() == co_gdut.read()) && (s_fdut.read() == s_gdut.read()))){
                    detected = true;
                    std::cout << "detected" << std::endl;
                    //--- write report ----------------------
                    reportFile << ", detected by testVector = " << testData << " @ " << sc_core::sc_time_stamp() << std::endl;
                } //--- endif: test detected the fault
            }//--- endfor: testvectors
            if(detected)
                numOfDetecteds++;
            
            accessRegistry->removeFaultList(flist, i); 
            wait(1, SC_NS);   
            std::cout << "remove fault:--------------------- " << std::endl;
        }//--- endfor: faultlist


        //--- calculate coverage ----------------------
        coverage = (numOfDetecteds / numOfFaults);

        //--- write report ----------------------
        reportFile << "numOfDetecteds: " << numOfDetecteds << std::endl;
        reportFile << "numOfFaults: " << numOfFaults << std::endl;
        reportFile << "coverage: " << coverage << std::endl;

        //--- close report file ----------------------
        reportFile.close();

        // wait(SC_ZERO_TIME);
        std::cout << "end of faultInjection::---------------------------  " << std::endl;
    }
};