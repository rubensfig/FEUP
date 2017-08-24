#ifndef HEADER_FILE
#define HEADER_FILE

#include <plib.h>
#include <inttypes.h>

uint8_t data[8];
static int led_int = 5;

#define FPB 80000000 // Frequency Peripheral Bus = 80MHz
#define BAUDRATE 115200

#define ON 15
#define PDSEL 1
#define STSEL 0
#define BRGH 3
#define UTXEN 10
#define URXEN 12
#define SAMP 1
#define DONE 0
#define PRINT_HEX 0
#define PRINT_DEC 1
#define CMD_SIZE 10

void init_ADC(void);
void acquire_ADC(void);
void init_uart(void);
void myputchar(char c);
char mygetchar(void);
void write_str(char *p) ;
void write_dec(uint8_t b);
void write_hex(uint8_t b);
void print_data(uint8_t base);
void __ISR(_TIMER_1_VECTOR, ipl2) Timer1Handler(void);
void __ISR(_TIMER_2_VECTOR, ipl3) Timer2Handler(void);
void set_motor(char signal, char n1, char n2);

#endif