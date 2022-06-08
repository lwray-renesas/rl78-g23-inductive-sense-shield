MEMORY
{
	VEC : ORIGIN = 0x0, LENGTH = 4
	IVEC : ORIGIN = 0x4, LENGTH = 188
	OPT : ORIGIN = 0xC0, LENGTH = 4
	SEC_ID : ORIGIN = 0xC4, LENGTH = 10
	OCDSTAD : ORIGIN = 0xCE, LENGTH = 10
	OCDROM : ORIGIN = 0x1fe00, LENGTH = 0x200
	ROM : ORIGIN = 0xD8, LENGTH = 130344
	MIRROR : ORIGIN = 0xF3000, LENGTH = 36608
	RAM : ORIGIN = 0xFBF00, LENGTH = 16384
}
SECTIONS
{
	.vec 0x0: AT(0x0)
	{
		KEEP(*(.vec))
	} > VEC
	.vects 0x4: AT(0x4)
	{
		KEEP(*(.vects))
	} > IVEC
	.option_bytes 0xc0 : AT(0xc0)
	{
		KEEP(*(.option_bytes))
	} > OPT
	.security_id 0xc4 : AT(0xc4)
	{
		KEEP(*(.security_id))
	} > SEC_ID
	.lowtext 0xD8: AT(0xD8)
	{
		*(.plt)
		*(.lowtext)
		. = ALIGN(2);
	} > ROM
	.tors :
	{
		__CTOR_LIST__ = .;
		. = ALIGN(2);
		___ctors = .;
		KEEP(*(.ctors))
		___ctors_end = .;
		__CTOR_END__ = .;
		__DTOR_LIST__ = .;
		___dtors = .;
		KEEP(*(.dtors))
		___dtors_end = .;
		__DTOR_END__ = .;
		. = ALIGN(2);
		_mdata = .;
	} > ROM
	.text (. + __romdatacopysize): 
	{
		*(.text)
		*(.text.*)
		etext = .;
		. = ALIGN(2);
	} > ROM AT> ROM 
	PROVIDE(__rl78_abs__ = 0);
	.init :
	{
		*(.init)
		*(.RFD_DATA_init)
	} > ROM
	.fini :
	{
		KEEP(*(.fini))
	} > ROM
	.got :
	{
		*(.got)
		*(.got.plt)
	} > ROM
	.rodata MAX(., 0x3000): 
	{
		. = ALIGN(2);
		*(.rodata)
		*(.RFD_CMN)
		*(.RFD_DF)
		*(.SMP_CMN)
		*(.SMP_DF)
		*(.rodata.*)
		_erodata = .;
	} > ROM
	.frodata :
	{
		. = ALIGN(2);
		*(.frodata)
		*(.frodata.*)
		_efrodata = .;
	} > ROM
	.eh_frame_hdr :
	{
		*(.eh_frame_hdr)
	} > ROM
	.eh_frame :
	{
		*(.eh_frame)
	} > ROM
	.jcr :
	{
		*(.jcr)
		. = ALIGN(2);
	} > ROM
	.data 0xFBF00: AT(_mdata)
	{
		. = ALIGN(2);
		__data = .;
		*(.data)
		*(.RFD_DATA)
		*(.data.*)
		. = ALIGN(2);
		_edata = .;
	} > RAM
	PROVIDE(__romdatacopysize = SIZEOF(.data));
	.bss :
	{
		. = ALIGN(2);
		_bss = .;
		*(.bss)
		*(.bss.**)
		. = ALIGN(2);
		*(COMMON)
		. = ALIGN(2);
		_ebss = .;
		__end = .;
	} > RAM AT> RAM 
	PROVIDE(stack_size = 0x64);
	.stack 0xFFEDC (NOLOAD) : AT(0xFFEDC)
	{
		__stack = .;
		ASSERT((__stack > (__end + stack_size)), "Error: Too much data - no room left for the stack");
	} > RAM
}
