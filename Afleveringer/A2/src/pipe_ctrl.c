#include "pipe_ctrl.h"
#include "arithmetic.h"

void fetch_wr_enable(fetch_regs* f, bool enable) {
  f->ip.wr_enable = enable;
}

void compute_wr_enable(compute_regs* c, bool enable) {
  c->is_valid.wr_enable = enable;
  c->major_op.wr_enable = enable;
  c->minor_op.wr_enable = enable;
  c->reg_d.wr_enable = enable;
  c->reg_s.wr_enable = enable;
  c->reg_z.wr_enable = enable;
  c->shamt.wr_enable = enable;
  c->imm_i.wr_enable = enable;
  c->imm_p.wr_enable = enable;
  c->next_ip.wr_enable = enable;
}

void load_store_wr_enable(load_store_regs* ls, bool enable) {
  ls->is_valid.wr_enable = enable;
  ls->is_load.wr_enable = enable;
  ls->is_store.wr_enable = enable;
  ls->addr.wr_enable = enable;
  ls->value.wr_enable = enable;
  ls->reg_d.wr_enable = enable;
}


#define CLK(reg) if (reg.wr_enable) { reg.out = reg.in; }
void fetch_clk(fetch_regs* f) {
  CLK(f->ip);
}

void compute_clk(compute_regs* c) {
  CLK(c->is_valid);
  CLK(c->major_op);
  CLK(c->minor_op);
  CLK(c->reg_d);
  CLK(c->reg_s);
  CLK(c->reg_z);
  CLK(c->shamt);
  CLK(c->imm_i);
  CLK(c->imm_p);
  CLK(c->next_ip);
}

void load_store_clk(load_store_regs* ls) {
  CLK(ls->is_valid);
  CLK(ls->is_load);
  CLK(ls->is_store);
  CLK(ls->addr);
  CLK(ls->value);
  CLK(ls->reg_d);
}

pipeline_control control_pipeline(fetch_regs* fetch, compute_regs* compute, load_store_regs* load_store, 
                                  selected_events* events) {

    pipeline_control result;
    //needs to determine if ifcr is valid
    bool is_flow_change_request_valid = events->insn_flow_change_request 
	                                && compute->is_valid.out; 
    bool compute_data_hazard = (same(load_store->reg_d.out,compute->reg_s.out) 
                                || same(load_store->reg_d.out,compute->reg_z.out) 
                                || same(load_store->reg_d.out,compute->reg_d.out)) 
                                && load_store->is_load.out;
    
    // Decide which pipeline registers to update (accept new instruction at clk boundary)
    result.load_store_runs = events->data_access_ok;
    result.compute_runs = result.load_store_runs && !compute_data_hazard;
    result.fetch_runs = result.compute_runs && events->insn_access_ok;

    // Decide which instructions to keep/potentially pass on/drop
    result.fetch_valid = result.fetch_runs && !events->insn_flow_change_request;
    compute->is_valid.in = result.fetch_valid;
    load_store->is_valid.in = compute->is_valid.out;

    

    // The "xxx_runs" signals then control update of pipeline registers:
    // (actual update happens when main() calls the "xxx_clk" functions at the end of the main loop)
    load_store_wr_enable(load_store, result.load_store_runs);
    compute_wr_enable(compute, result.compute_runs);
    fetch_wr_enable(fetch, result.fetch_runs);

    return result;
}
