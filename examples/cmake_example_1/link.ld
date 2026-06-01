/* linker.ld */
OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)

ENTRY(_start)

PHDRS
{
  ro PT_LOAD;
  rw PT_LOAD;
}

MEMORY
{
    /* Cortex-R52 TCM 示例地址，请根据实际芯片手册确认 */
    TCM_A (RX) : ORIGIN = 0x60000000, LENGTH = 0x00020000  /* 128KB: 代码+只读数据 */
    TCM_B (RW) : ORIGIN = 0x60020000, LENGTH = 0x0001F800 /* 128KB: 数据+栈(尾端) */
}

SECTIONS
{
    /* 代码段从 TCM_A 起始地址开始 */
    . = ORIGIN(TCM_A);
    
    PROVIDE (__executable_start = .);

    /* 丢弃不需要的段 */
    /DISCARD/ :
    {
        *(.note.gnu.build-id)
        *(.hash)
        *(.gnu.hash)
        *(.dynsym)
        *(.dynstr)
        *(.gnu.version)
        *(.gnu.version_d)
        *(.gnu.version_r)
        *(.rel.init)
        *(.rela.init)
        *(.rel.text .rel.text.* .rel.gnu.linkonce.t.*)
        *(.rela.text .rela.text.* .rela.gnu.linkonce.t.*)
        *(.rel.fini)
        *(.rela.fini)
        *(.rel.rodata .rel.rodata.* .rel.gnu.linkonce.r.*)
        *(.rela.rodata .rela.rodata.* .rela.gnu.linkonce.r.*)
        *(.rel.data.rel.ro* .rel.gnu.linkonce.d.rel.ro.*)
        *(.rela.data.rel.ro* .rela.gnu.linkonce.d.rel.ro.*)
        *(.rel.data .rel.data.* .gnu.linkonce.d.*)
        *(.rela.data .rela.data.* .gnu.linkonce.d.*)
        *(.rel.tdata .rel.tdata.* .gnu.linkonce.td.*)
        *(.rela.tdata .rela.tdata.* .gnu.linkonce.td.*)
        *(.rel.tbss .rel.tbss.* .gnu.linkonce.tb.*)
        *(.rela.tbss .rela.tbss.* .gnu.linkonce.tb.*)
        *(.rel.ctors)
        *(.rela.ctors)
        *(.rel.dtors)
        *(.rela.dtors)
        *(.rel.got)
        *(.rela.got)
        *(.rel.bss .rel.bss.* .gnu.linkonce.b.*)
        *(.rela.bss .rela.bss.* .gnu.linkonce.b.*)
        *(.rel.plt)
        *(.rela.plt)
        *(.plt)
        *(.iplt)
    }

    /* 
     * .text 段：包含向量表和代码 
     */
    .text :
    {
        _stext = .;
        _prog_img_start = .;
        
        /* 确保向量表在最开头 */
        KEEP(*(.vectors))
        
        *(.text.init)
        *(.init)
        *(.text .text.* .gnu.linkonce.t.*)
        
        /* C++ 异常处理相关 */
        KEEP (*(.text.*personality*))
        *(.glue_7t) *(.glue_7)
        *(.gnu.warning)
    } > TCM_A :ro

    .init :
    {
        KEEP (*(.init))
    } > TCM_A :ro =0x90909090

    .fini :
    {
        KEEP (*(.fini))
    } > TCM_A :ro = 0x9090

    PROVIDE (_etext = .);
    PROVIDE (etext = .);

    /* 只读数据段 */
    .rodata : 
    { 
        *(.rodata .rodata.* .gnu.linkonce.r.*) 
        . = ALIGN(4);
    } > TCM_A :ro

    .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } > TCM_A :ro
    __exidx_start = .;
    .ARM.exidx : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) } > TCM_A :ro
    __exidx_end = .;

    /* 
     * .data 段：已初始化的全局变量 
     */
    . = ALIGN(4);
    PROVIDE (_sdata = .);
    
    .data :
    {
        *(.data .data.* .gnu.linkonce.d.*)
        KEEP (*(.gnu.linkonce.d.*personality*))
        SORT(CONSTRUCTORS)
        . = ALIGN(4);
    } > TCM_B :rw

    _edata = .; PROVIDE (edata = .);

    /* .bss 段：未初始化的全局变量，放在 TCM_B */
    __bss_start = .;
    .bss :
    {
        *(.dynbss)
        *(.bss .bss.* .gnu.linkonce.b.*)
        *(COMMON)
        . = ALIGN(4);
    } > TCM_B :rw
    
    . = ALIGN(4);
    _end = .; PROVIDE (end = .);
    __bss_end = .;

    /* 栈放在 TCM_B 末尾 */
    _estack = ORIGIN(TCM_B) + LENGTH(TCM_B);
    _Lstack = _estack - 8;

    /* 调试信息通常不需要放入加载的程序头中 */
    .debug 0 : { *(.debug) }
    .line 0 : { *(.line) }
    .debug_srcinfo 0 : { *(.debug_srcinfo) }
    .debug_sfnames 0 : { *(.debug_sfnames) }
    .debug_aranges 0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    .debug_info 0 : { *(.debug_info .gnu.linkonce.wi.*) }
    .debug_abbrev 0 : { *(.debug_abbrev) }
    .debug_line 0 : { *(.debug_line) }
    .debug_frame 0 : { *(.debug_frame) }
    .debug_str 0 : { *(.debug_str) }
    .debug_loc 0 : { *(.debug_loc) }
    .debug_macinfo 0 : { *(.debug_macinfo) }
    .debug_weaknames 0 : { *(.debug_weaknames) }
    .debug_funcnames 0 : { *(.debug_funcnames) }
    .debug_typenames 0 : { *(.debug_typenames) }
    .debug_varnames 0 : { *(.debug_varnames) }
}