
	.section .rodata

	.global	m0_rx_bin
	.align  4
m0_rx_bin:
	.incbin "../m0_rx/m0rx.bin"
	.global	m0_tx_bin
	.align  4
m0_tx_bin:
	.incbin "../m0_tx/m0tx.bin"
	
	.global m0_rx_bin_size
	.align  4
m0_rx_bin_size:
	.int	m0_tx_bin - m0_rx_bin
	
	.global m0_tx_bin_size
	.align  4
m0_tx_bin_size:
	.int	m0_rx_bin_size - m0_tx_bin
