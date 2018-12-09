# mruby-tlsf

`mruby-tlsf` uses [TLSF memory allocator written by Matthew Conte](https://github.com/mattconte/tlsf) as a custom allocator in mruby.

## Usage

This mrbgem defines `mrb_state* mrb_open_tlsf(void *mem, size_t bytes)`.
You can use this instead of `mrb_open()`.

## Methods

* `TLSF::available_memory` : returns available memory.
* `TLSF::used_memory` : returns used memory.
* `TLSF::total_memory` : returns total memory.

## LICENSE

MIT

