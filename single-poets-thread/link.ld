/* THIS FILE HAS BEEN GENERATED AUTOMATICALLY. */
/* DO NOT MODIFY. INSTEAD, MODIFY THE genld.sh SCRIPT. */

OUTPUT_ARCH( "riscv" )

MEMORY
{
  instrs  : ORIGIN = 576, LENGTH = 7616
  globals : ORIGIN = 25165824, LENGTH = 1048576000
}

SECTIONS
{
  .text   : { *.o(.text*) }             > instrs
  .bss    : { *.o(.bss*) }              > globals = 0
  .rodata : { *.o(.rodata*) }           > globals
  .sdata  : { *.o(.sdata*) }            > globals
  .data   : { *.o(.data*) }             > globals
  __heapBase = ALIGN(.);
}
