#include "systemc.h"
#include "testbench.h"

int sc_main(int argc, char* argv[]){
    testbench* fulladder_tb = new testbench("fulladder_tb");
    

    sc_trace_file *vcdfile;
    vcdfile = sc_create_vcd_trace_file("vcdfile");
    vcdfile->set_time_unit(1, SC_NS);

    sc_trace(vcdfile, fulladder_tb->i0, "i0");
    sc_trace(vcdfile, fulladder_tb->i1, "i1");
    sc_trace(vcdfile, fulladder_tb->ci, "ci");
    sc_trace(vcdfile, fulladder_tb->s_gld, "s_gld");
    sc_trace(vcdfile, fulladder_tb->s_flt, "s_flt");
    sc_trace(vcdfile, fulladder_tb->co_gld, "co_gld");
    sc_trace(vcdfile, fulladder_tb->co_flt, "co_flt");
    sc_trace(vcdfile, fulladder_tb->gld_dut->nand_n_7->in1[0], "gld->nand_n_7->in1[0]");
    sc_trace(vcdfile, fulladder_tb->gld_dut->nand_n_7->in1[1], "gld->nand_n_7->in1[1]");
    sc_trace(vcdfile, fulladder_tb->flt_dut->nand_n_7->in1[0], "flt->nand_n_7->in1[0]");
    sc_trace(vcdfile, fulladder_tb->flt_dut->nand_n_7->in1[1], "flt->nand_n_7->in1[1]");
    sc_trace(vcdfile, fulladder_tb->gld_dut->nand_n_8->in1[0], "gld->nand_n_8->in1[0]");
    sc_trace(vcdfile, fulladder_tb->gld_dut->nand_n_8->in1[1], "gld->nand_n_8->in1[1]");
    sc_trace(vcdfile, fulladder_tb->flt_dut->nand_n_8->in1[0], "flt->nand_n_8->in1[0]");
    sc_trace(vcdfile, fulladder_tb->flt_dut->nand_n_8->in1[1], "flt->nand_n_8->in1[1]");
    sc_trace(vcdfile, fulladder_tb->gld_dut->nand_n_6->in1[0], "gld->nand_n_6->in1[0]");
    sc_trace(vcdfile, fulladder_tb->gld_dut->nand_n_6->in1[1], "gld->nand_n_6->in1[1]");
    sc_trace(vcdfile, fulladder_tb->flt_dut->nand_n_6->in1[0], "flt->nand_n_6->in1[0]");
    sc_trace(vcdfile, fulladder_tb->flt_dut->nand_n_6->in1[1], "flt->nand_n_6->in1[1]");
 
    sc_start(1000, SC_NS);
    return 0;
}
