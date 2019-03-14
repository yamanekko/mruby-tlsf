#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mruby.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include <mruby/compile.h>
#include "mruby_tlsf.h"

#ifndef MRUBY_TLSF_INIT_MEM
#define MRUBY_TLSF_INIT_MEM 1000000
#endif

int
main(int argc, char **argv)
{
  mrb_state *mrb;
  int ret = 0;
  char *mem[MRUBY_TLSF_INIT_MEM];

  mrb = mrb_open_tlsf((void *)mem, MRUBY_TLSF_INIT_MEM);
  if (mrb == NULL) {
    fprintf(stderr, "Invalid mrb_state, exiting %s\n", argv[0]);
    return EXIT_FAILURE;
  }

  mrb_load_string(mrb, "printf \"total: %8d\\n\", TLSF::total_memory");
  mrb_load_string(mrb, "printf \"used:  %8d\\n\", TLSF::used_memory");
  mrb_load_string(mrb, "printf \"free:  %8d\\n\", TLSF::available_memory");
  mrb_load_string(mrb, "p 'hello world!'");
  mrb_load_string(mrb, "printf \"used:  %8d\\n\", TLSF::used_memory");
  mrb_load_string(mrb, "printf \"free:  %8d\\n\", TLSF::available_memory");

  mrb_close_tlsf(mrb);
  return ret;
}
