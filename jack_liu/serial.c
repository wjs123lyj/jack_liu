/*
 * (C) Copyright 2002
 * Gary Jennejohn, DENX Software Engineering, <gj@denx.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <common.h>

#include <s5pc110.h>

#ifdef CONFIG_SERIAL1
#define UART_NR	S5PC11X_UART0

#elif defined(CONFIG_SERIAL2)
#define UART_NR	S5PC11X_UART1

#elif defined(CONFIG_SERIAL3)
#define UART_NR	S5PC11X_UART2

#elif defined(CONFIG_SERIAL4)
#define UART_NR S5PC11X_UART3

#else
#error "Bad: you didn't configure serial ..."
#endif

#include "../loader/incl/loader.h"


void serial_setbrg(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	int i;
	for (i = 0; i < 100; i++);
}

/*
 * Initialise the serial port with the given baudrate. The settings
 * are always 8 data bits, no parity, 1 stop bit, no start bits.
 *
 */
int serial_init(void)
{
	serial_setbrg();

	return (0);
}

/*
 * Read a single byte from the serial port. Returns 1 on success, 0
 * otherwise. When the function is succesfull, the character read is
 * written into its argument c.
 */
int serial_getc(void)
{
	S5PC11X_UART *const uart = S5PC11X_GetBase_UART(UART_NR);

	/* wait for character to arrive */
	while (!(uart->UTRSTAT & 0x1));

	return uart->URXH & 0xff;
}


//king add, 13-12-31
int serial_getchar_nr(int nr)
{
    S5PC11X_UART *const uart = S5PC11X_GetBase_UART(nr);

	/* wait for character to arrive */
	while (!(uart->UTRSTAT & 0x1));

	return uart->URXH & 0xff;
}

#ifdef CONFIG_HWFLOW
static int hwflow = 0;		/* turned off by default */
int hwflow_onoff(int on)
{
	switch (on) {
	case 0:
	default:
		break;		/* return current */
	case 1:
		hwflow = 1;	/* turn on */
		break;
	case -1:
		hwflow = 0;	/* turn off */
		break;
	}
	return hwflow;
}
#endif

#ifdef CONFIG_MODEM_SUPPORT
static int be_quiet = 0;
void disable_putc(void)
{
	be_quiet = 1;
}

void enable_putc(void)
{
	be_quiet = 0;
}
#endif


/*
 * Output a single byte to the serial port.
 */
void serial_putc(const char c)
{
	S5PC11X_UART *const uart = S5PC11X_GetBase_UART(UART_NR);

#ifdef CONFIG_MODEM_SUPPORT
	if (be_quiet)
		return;
#endif

	/* wait for room in the tx FIFO */
	while (!(uart->UTRSTAT & 0x2));

#ifdef CONFIG_HWFLOW
	/* Wait for CTS up */
	while (hwflow && !(uart->UMSTAT & 0x1));
#endif

	uart->UTXH = c;

	/* If \n, also do \r */
	if (c == '\n')
		serial_putc('\r');
}

/*
 * Test whether a character is in the RX buffer
 */
int serial_tstc(void)
{
	S5PC11X_UART *const uart = S5PC11X_GetBase_UART(UART_NR);

	return uart->UTRSTAT & 0x1;
}

void serial_puts(const char *s)
{
	while (*s) {
		serial_putc(*s++);
	}
}

//king add, 14-1-02
void serial_putc_of_nr(int portnum, const char c)
{
	S5PC11X_UART *const uart = S5PC11X_GetBase_UART(portnum);

#ifdef CONFIG_MODEM_SUPPORT
	if (be_quiet)
		return;
#endif

	/* wait for room in the tx FIFO */
	while (!(uart->UTRSTAT & 0x2));

#ifdef CONFIG_HWFLOW
	/* Wait for CTS up */
	while (hwflow && !(uart->UMSTAT & 0x1));
#endif

	uart->UTXH = c;

	/* If \n, also do \r */
	if (c == '\n')
		serial_putc('\r');
}

void serial_puts_of_nr(int portnum, const char *s, int count)
{
    #if 0
    while (*s) {
        serial_putc_of_nr(portnum, *s++);
    }
    #endif

    for (; count>0; count--) {
        serial_putc_of_nr(portnum, *s++);
    }
}

//king add, 14-01-02
int serial_tstc_of_nr(int nr)
{
    S5PC11X_UART *const uart = S5PC11X_GetBase_UART(nr);

	return uart->UTRSTAT & 0x1;
}

//king add, 13-12-31
unsigned int pl01x_peekc (int portnum)
{
    static int cnt;
	S5PC11X_UART *const uart = S5PC11X_GetBase_UART(portnum);

    if( serial_tstc_of_nr(portnum) )
    {
        unsigned int data;
        data = uart->URXH & 0xff;
#if (SERIAL_C_DEG > 0)
        printf("received data is %d\n", data);
#endif
    	return data;
    }
    else
    {
#if (SERIAL_C_DEG > 0)
        cnt++;
        if (cnt%2000 == 0) {
            cnt = 0;
            printf("%s error\n", __func__);
        }
#endif
        return 0xFFFFFFFF;
    }
}

//king add, 13-12-31
//Actually, only baudrate 9600 and 115200 are supported.
int Serial_BaudRateSet(int num, int baudRate)
{
#if (SERIAL_C_DEG > 0)
    printf("===%s===\n", __func__);
#endif

	S5PC11X_UART *const uart = S5PC11X_GetBase_UART(num);

    uart->UFCON = 0;

    uart->UMCON = 0;
    /*Set the UART to be 8 bits, 1 stop bit, no parity*/
    uart->ULCON = 0x3;

    /*Rx/Tx polling mode, clock: PCLK*/
    uart->UCON  = 0x5;

    if (baudRate == 9600) {
        uart->UBRDIV = 432; 
    } else if (baudRate == 115200) {
        uart->UBRDIV = 34;
        IO_WRITE(0xE2900000+num*0x400 + 0x2C, 0xDDDD);
    } else {
        printf("baudrate unknown!\n");
    }

    return 0;
}

//king add, 14-01-02
void uart1_perh_init(void)
{
    unsigned int reg;

    reg = IO_READ(GPA0CON_ADDR);

    reg |= (2<<16) | (2<<20);

    /*io to uart1_txd and uart1_rxd*/
    IO_WRITE(GPA0CON_ADDR, reg);

}


//king add, 14-02-20
//Initialise all serial ports.
void uart_x_perh_init(int x)
{
    unsigned int tmp;

    switch (x) {
    case 0: // UART0
        tmp = IO_READ(GPA0CON_ADDR);
        tmp &= 0xFFFFFF00; // clear low-8 bit.
        tmp |= (2<<4) | (2<<0);
        IO_WRITE(GPA0CON_ADDR, tmp);
        break;

    case 1: // UART1
        tmp = IO_READ(GPA0CON_ADDR);
        tmp &= 0xFF00FFFF; // clear bit[23:16].
        tmp |= (2<<20) | (2<<16);
        IO_WRITE(GPA0CON_ADDR, tmp);
        break;

    case 2: // UART2
        tmp = IO_READ(GPA1CON_ADDR);
        tmp &= 0xFF00; // clear low-8 bit.
        tmp |= (2<<4) | (2<<0);
        IO_WRITE(GPA1CON_ADDR, tmp);
        break;

    case 3: // UART3
        tmp = IO_READ(GPA1CON_ADDR);
        tmp &= 0x00FF; // clear bit[15:8].
        tmp |= (2<<8) | (2<<12);
        IO_WRITE(GPA1CON_ADDR, tmp);
        break;
    }

    return;
}


#if (SERIAL_C_DEG > 0)
void test_uart1(void)
{
    char c;
    c = serial_getchar_nr(1);

    serial_putc_of_nr(0, c);
}
#endif





