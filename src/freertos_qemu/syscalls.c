/*
 * FreeRTOS V202212.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */
#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <stdint.h>
#include <sys/stat.h>
#include <errno.h>

typedef struct UART_t
{
    volatile uint32_t DATA;
    volatile uint32_t STATE;
    volatile uint32_t CTRL;
    volatile uint32_t INTSTATUS;
    volatile uint32_t BAUDDIV;
} UART_t;

#define UART0_ADDR         ( ( UART_t * ) ( 0x40004000 ) )
#define UART_DR( baseaddr )    ( *( unsigned int * ) ( baseaddr ) )

#define UART_CTRL_TX_EN    ( 1 << 0 )

extern unsigned long _heap_bottom;
extern unsigned long _heap_top;

static char * heap_end = ( char * ) &_heap_bottom;

/**
 * @brief initializes the UART emulated hardware
 */
void uart_init( void )
{
    UART0_ADDR->BAUDDIV = 16;
    UART0_ADDR->CTRL = UART_CTRL_TX_EN;
}

int _close(int file) 
{ 
    (void)file;
    return -1; 
}

int _fstat(int file, struct stat *st) 
{ 
    (void)file;
    st->st_mode = S_IFCHR; 
    return 0; 
}

int _getpid(void) 
{ 
    return 1; 
}

int _isatty(int file) 
{ 
    (void)file;
    return 1; 
}

int _kill(int pid, int sig) 
{ 
    (void)pid;
    (void)sig;
    errno = EINVAL; 
    return -1; 
}

int _lseek(int file, int ptr, int dir) 
{ 
    (void)file;
    (void)ptr;
    (void)dir;
    return 0; 
}

int _read(int file, char *ptr, int len)
{
    (void)file;
    (void)ptr;
    (void)len;
    return 0;
}

int _write(int file, char *buf, int len)
{
    int todo;
    (void)file;

    for(todo = 0; todo < len; todo++)
    {
        UART_DR(UART0_ADDR) = *buf++;
    }

    return len;
}

#ifdef __cplusplus
}
#endif
