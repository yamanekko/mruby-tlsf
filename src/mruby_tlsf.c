#include <mruby.h>
#include <stdio.h>
#include "tlsf.h"
#include "mruby_tlsf.h"

typedef struct mrb_tlsf_t {
  tlsf_t tlsf;
  mrb_int total;
} mrb_tlsf_t;

/**
 *
 * return 1 or 0 if pointer is in TLSF pool area or not
 *
 */
mrb_int
mrb_tlsf_managed_addr_p(mrb_state *mrb, void *ptr)
{
  struct mrb_tlsf_t *t = (struct mrb_tlsf_t*)mrb->allocf_ud;
  if (((intptr_t)t->tlsf < (intptr_t)ptr) && ((intptr_t)ptr < (intptr_t)t->tlsf + t->total)) {
    return 1;
  } else {
    return 0;
  }
}

/**
 *
 * allocation function using TLSF.
 *
 */
MRB_API void*
mrb_tlsf_allocf(mrb_state *mrb, void *p, size_t size, void *ud)
{
  mrb_tlsf_t *t = (mrb_tlsf_t*)ud;
  void *p2;

  if (size == 0) {
    if (p) {
#ifdef MRB_TLSF_DEBUG
      fprintf(stdout, "free:    (%p)\n", p);
#endif
      if (mrb_tlsf_managed_addr_p(mrb, p)) {
        tlsf_free(t->tlsf, p);
      } else {
        // ignore if non-TLSF address
      }
    }
    return NULL;
  }
  else {
    // use realloc if TLSF pointer, or use malloc if non-TLSF pointer
    if (mrb && mrb_tlsf_managed_addr_p(mrb, p)) {
      p2 = tlsf_realloc(t->tlsf, p, size);
    } else {
      p2 = tlsf_malloc(t->tlsf, size);
    }
#ifdef MRB_TLSF_DEBUG
    if (p) {
      fprintf(stdout, "realloc: (%p) %zu\n", p, size);
    } else {
      fprintf(stdout, "malloc:  (%p) %zu\n", p2, size);
    }
#endif
    return p2;
  }
}

/**
 *
 * initialize TLSF structure.
 *
 */
MRB_API struct mrb_tlsf_t*
mrb_tlsf_init(void *mem, size_t bytes)
{
  tlsf_t tlsf = tlsf_create_with_pool(mem, bytes);
  mrb_tlsf_t *t = (mrb_tlsf_t *)tlsf_malloc(tlsf, sizeof(mrb_tlsf_t));
  t->tlsf = tlsf;
  t->total = bytes;
  return t;
}

/**
 *
 * <code>mrb_open()</code> using TLSF.
 *
 */
MRB_API mrb_state*
mrb_open_tlsf(void *mem, size_t bytes)
{
  struct mrb_tlsf_t *t = mrb_tlsf_init(mem, bytes);
  mrb_state *mrb = mrb_open_allocf(mrb_tlsf_allocf, (void *)t);

  return mrb;
}

/**
 *
 * <code>mrb_close()</code> using TLSF.
 *
 */
MRB_API void
mrb_close_tlsf(mrb_state *mrb)
{
  mrb_tlsf_t *t = (mrb_tlsf_t*)mrb->allocf_ud;
  mrb_close(mrb);
  tlsf_t tlsf = t->tlsf;
  tlsf_free(t->tlsf, t);
  tlsf_destroy(tlsf);
  return;
}

static void
mrb_tlsf_memory_walker(void* ptr, size_t size, int used, void* user)
{
  mrb_int *mem = (mrb_int*)user;
  if (!used) {
    *mem += size;
  }
}

/**
 *  call-seq:
 *     TLSF.total_memory  => int
 *
 *  Returns size of total memory.
 */
MRB_API mrb_value
mrb_tlsf_total_memory(mrb_state *mrb, mrb_value obj)
{
  mrb_tlsf_t *t = (mrb_tlsf_t*)mrb->allocf_ud;
  return mrb_fixnum_value(t->total);
}

/**
 *  call-seq:
 *     TLSF.used_memory  => int
 *
 *  Returns size of used memory.
 */
MRB_API mrb_value
mrb_tlsf_used_memory(mrb_state *mrb, mrb_value obj)
{
  mrb_int available = 0;
  mrb_tlsf_t *t = (mrb_tlsf_t*)mrb->allocf_ud;
  pool_t pool = tlsf_get_pool(t->tlsf);
  tlsf_walk_pool(pool, mrb_tlsf_memory_walker, (void *)&available);

  return mrb_fixnum_value(t->total - available);
}

/**
 *  call-seq:
 *     TLSF.available_memory  => int
 *
 *  Returns size of available memory.
 */
MRB_API mrb_value
mrb_tlsf_available_memory(mrb_state *mrb, mrb_value obj)
{
  mrb_int available = 0;
  mrb_tlsf_t *t = (mrb_tlsf_t*)mrb->allocf_ud;
  pool_t pool = tlsf_get_pool(t->tlsf);
  tlsf_walk_pool(pool, mrb_tlsf_memory_walker, (void *)&available);

  return mrb_fixnum_value(available);
}

void
mrb_mruby_tlsf_gem_init(mrb_state* mrb) {
  struct RClass *m = mrb_define_module(mrb, "TLSF");
  mrb_define_class_method(mrb, m, "available_memory", mrb_tlsf_available_memory, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, m, "total_memory", mrb_tlsf_total_memory, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, m, "used_memory", mrb_tlsf_used_memory, MRB_ARGS_NONE());
}

void
mrb_mruby_tlsf_gem_final(mrb_state* mrb) {
  /* finalizer */
}
