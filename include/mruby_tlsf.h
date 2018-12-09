/*
** mruby_tlsf.h
*/

#ifndef MRUBY_TLSF_H
#define MRUBY_TLSF_H

MRB_API mrb_state* mrb_open_tlsf(void *mem, size_t bytes);
MRB_API void* mrb_tlsf_allocf(mrb_state *mrb, void *p, size_t size, void *ud);
MRB_API struct mrb_tlsf_t* mrb_tlsf_init(void *mem, size_t bytes);
MRB_API void mrb_close_tlsf(mrb_state *mrb);

MRB_API mrb_value mrb_tlsf_total_memory(mrb_state *mrb, mrb_value obj);
MRB_API mrb_value mrb_tlsf_used_memory(mrb_state *mrb, mrb_value obj);
MRB_API mrb_value mrb_tlsf_available_memory(mrb_state *mrb, mrb_value obj);

#endif  /* MRUBY_TLSF_H */

