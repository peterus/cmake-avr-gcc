/************************************************************************
Title:    Interrupt UART library with receive/transmit circular buffers
Author:   Peter Fleury <pfleury@gmx.ch>   http://jump.to/fleury
File:     $Id: uart.h,v 1.12 2012/11/19 19:52:27 peter Exp $
Software: AVR-GCC 4.1, AVR Libc 1.4
Hardware: any AVR with built-in UART, tested on AT90S8515 & ATmega8 at 4 Mhz
License:  GNU General Public License 
Usage:    see Doxygen manual

LICENSE:
    Copyright (C) 2006 Peter Fleury

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
	
	Changed from Peter Buchegger.
    
************************************************************************/


#ifndef UART_H_
#define UART_H_

/** @brief  UART Baudrate Expression
 *  @param  xtalcpu  system clock in Mhz, e.g. 4000000UL for 4Mhz          
 *  @param  baudrate baudrate in bps, e.g. 1200, 2400, 9600     
 */
#define UART_BAUD_SELECT(baudRate,xtalCpu)  (((xtalCpu) + 8UL * (baudRate)) / (16UL * (baudRate)) -1UL)

/** @brief  UART Baudrate Expression for ATmega double speed mode
 *  @param  xtalcpu  system clock in Mhz, e.g. 4000000UL for 4Mhz           
 *  @param  baudrate baudrate in bps, e.g. 1200, 2400, 9600     
 */
#define UART_BAUD_SELECT_DOUBLE_SPEED(baudRate,xtalCpu) ( ((((xtalCpu) + 4UL * (baudRate)) / (8UL * (baudRate)) -1UL)) | 0x8000)


/** Size of the circular receive buffer, must be power of 2 */
#ifndef UART_RX_BUFFER_SIZE
#define UART_RX_BUFFER_SIZE 32
#endif
/** Size of the circular transmit buffer, must be power of 2 */
#ifndef UART_TX_BUFFER_SIZE
#define UART_TX_BUFFER_SIZE 32
#endif

/* test if the size of the circular buffers fits into SRAM */
#if ( (UART_RX_BUFFER_SIZE+UART_TX_BUFFER_SIZE) >= (RAMEND-0x60 ) )
#error "size of UART_RX_BUFFER_SIZE + UART_TX_BUFFER_SIZE larger than size of SRAM"
#endif

/* 
** high byte error return code of uart_getc()
*/
#define UART_FRAME_ERROR      0x1000              /* Framing Error by UART       */
#define UART_OVERRUN_ERROR    0x0800              /* Overrun condition by UART   */
#define UART_PARITY_ERROR     0x0400              /* Parity Error by UART        */ 
#define UART_BUFFER_OVERFLOW  0x0200              /* receive ringbuffer overflow */
#define UART_NO_DATA          0x0100              /* no receive data available   */


/*
** function prototypes
*/

/**
   @brief   Initialize UART and set baudrate 
   @param   baudrate Specify baudrate using macro UART_BAUD_SELECT()
   @return  none
*/
extern void UART_Init(unsigned int baudrate);


/**
 *  @brief   Get received byte from ringbuffer
 *
 * Returns in the lower byte the received character and in the 
 * higher byte the last receive error.
 * UART_NO_DATA is returned when no data is available.
 *
 *  @param   void
 *  @return  lower byte:  received byte from ringbuffer
 *  @return  higher byte: last receive status
 *           - \b 0 successfully received data from UART
 *           - \b UART_NO_DATA           
 *             <br>no receive data available
 *           - \b UART_BUFFER_OVERFLOW   
 *             <br>Receive ringbuffer overflow.
 *             We are not reading the receive buffer fast enough, 
 *             one or more received character have been dropped 
 *           - \b UART_OVERRUN_ERROR     
 *             <br>Overrun condition by UART.
 *             A character already present in the UART UDR register was 
 *             not read by the interrupt handler before the next character arrived,
 *             one or more received characters have been dropped.
 *           - \b UART_FRAME_ERROR       
 *             <br>Framing Error by UART
 */
extern unsigned int UART_GetC(void);


/**
 *  @brief   Put byte to ringbuffer for transmitting via UART
 *  @param   data byte to be transmitted
 *  @return  none
 */
extern void UART_PutC(unsigned char data);
extern int UART_PutC_(char data, FILE * stream);


/**
 *  @brief   Put string to ringbuffer for transmitting via UART
 *
 *  The string is buffered by the uart library in a circular buffer
 *  and one character at a time is transmitted to the UART using interrupts.
 *  Blocks if it can not write the whole string into the circular buffer.
 * 
 *  @param   s string to be transmitted
 *  @return  none
 */
extern void UART_PutS(const char *s );


/**
 * @brief    Put string from program memory to ringbuffer for transmitting via UART.
 *
 * The string is buffered by the uart library in a circular buffer
 * and one character at a time is transmitted to the UART using interrupts.
 * Blocks if it can not write the whole string into the circular buffer.
 *
 * @param    s program memory string to be transmitted
 * @return   none
 * @see      UART_PutS_P
 */
extern void UART_PutS_P(const char *s );

/**
 * @brief    Macro to automatically put a string constant into program memory
 */
//#define UART_PutS_P(__s)       UART_PutS_P(PSTR(__s))



/** @brief  Initialize USART1 (only available on selected ATmegas) @see uart_init */
extern void UART1_Init(unsigned int baudrate);
/** @brief  Get received byte of USART1 from ringbuffer. (only available on selected ATmega) @see uart_getc */
extern unsigned int UART1_GetC(void);
/** @brief  Put byte to ringbuffer for transmitting via USART1 (only available on selected ATmega) @see uart_putc */
extern void UART1_PutC(unsigned char data);
/** @brief  Put string to ringbuffer for transmitting via USART1 (only available on selected ATmega) @see uart_puts */
extern void UART1_PutS(const char *s );
/** @brief  Put string from program memory to ringbuffer for transmitting via USART1 (only available on selected ATmega) @see UART_PutS_P */
extern void UART1_PutS_P(const char *s );
/** @brief  Macro to automatically put a string constant into program memory */
//#define UART1_PutS_P(__s)       UART1_PutS_P(PSTR(__s))

/**@}*/


#endif /* UART_H_ */
