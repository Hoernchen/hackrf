/*
* This file is part of the libopencm3 project.
*
* Copyright (C) 2010 Uwe Hermann <uwe@hermann-uwe.de>
* Copyright (C) 2012 Michael Ossmann <mike@ossmann.com>
* Copyright (C) 2012 Benjamin Vernoux <titanmkd@gmail.com>
*
* This library is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <libopencm3/lpc43xx/sgpio.h>
#include <libopencm3/lpc43xx/nvic.h>
#include <libopencm3/lpc43xx/creg.h>


uint8_t* const usb_bulk_buffer = (uint8_t*)0x20004000;
static volatile uint32_t usb_bulk_buffer_offset = 0;
static const uint32_t usb_bulk_buffer_mask = 32768 - 1;

int flag = 0;
void m0_sgpio_isr() {
	SGPIO_CLR_STATUS_1 = (1 << SGPIO_SLICE_A);
	uint32_t* const p = (uint32_t*)&usb_bulk_buffer[usb_bulk_buffer_offset];

		__asm__(
			"ldr r0, [%[SGPIO_REG_SS], #44]\n\t"
			"rev16 r0, r0\n\t" /* Swap QI -> IQ */
			"str r0, [%[p], #0]\n\t"
			"ldr r0, [%[SGPIO_REG_SS], #20]\n\t"
			"rev16 r0, r0\n\t" /* Swap QI -> IQ */
			"str r0, [%[p], #4]\n\t"
			"ldr r0, [%[SGPIO_REG_SS], #40]\n\t"
			"rev16 r0, r0\n\t" /* Swap QI -> IQ */
			"str r0, [%[p], #8]\n\t"
			"ldr r0, [%[SGPIO_REG_SS], #8]\n\t"
			"rev16 r0, r0\n\t" /* Swap QI -> IQ */
			"str r0, [%[p], #12]\n\t"
			"ldr r0, [%[SGPIO_REG_SS], #36]\n\t"
			"rev16 r0, r0\n\t" /* Swap QI -> IQ */
			"str r0, [%[p], #16]\n\t"
			"ldr r0, [%[SGPIO_REG_SS], #16]\n\t"
			"rev16 r0, r0\n\t" /* Swap QI -> IQ */
			"str r0, [%[p], #20]\n\t"
			"ldr r0, [%[SGPIO_REG_SS], #32]\n\t"
			"rev16 r0, r0\n\t" /* Swap QI -> IQ */
			"str r0, [%[p], #24]\n\t"
			"ldr r0, [%[SGPIO_REG_SS], #0]\n\t"
			"rev16 r0, r0\n\t" /* Swap QI -> IQ */
			"str r0, [%[p], #28]\n\t"
			:
			: [SGPIO_REG_SS] "l" (SGPIO_PORT_BASE + 0x100),
			  [p] "l" (p)
			: "r0"
		);
	
	usb_bulk_buffer_offset = (usb_bulk_buffer_offset + 32) & usb_bulk_buffer_mask;
	
	if(usb_bulk_buffer_offset == 0){
		flag = 0;
		__asm("sev;");
	}else if( !flag && usb_bulk_buffer_offset & 0x4000 ){
		flag = 1;
		__asm("sev;");
	}
}
int main(void)
{
	nvic_disable_irq(NVIC_M0_SGPIO_IRQ);
	nvic_clear_pending_irq(NVIC_M0_SGPIO_IRQ);
	nvic_set_priority(NVIC_M0_SGPIO_IRQ, 0);
	nvic_enable_irq(NVIC_M0_SGPIO_IRQ);
	
	while(1) __asm("wfe;");
	
	return 0;
}
