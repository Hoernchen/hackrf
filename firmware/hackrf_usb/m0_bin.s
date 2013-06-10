
	.section .rodata

	.global	m0_rxtx_bin
	.align  4
m0_rxtx_bin:
	.incbin "../m0_rxtx/m0rxtx.bin"
	
	.global m0_rxtx_bin_size
	.align  4
m0_rxtx_bin_size:
	.int	m0_rxtx_bin_size - m0_rxtx_bin
