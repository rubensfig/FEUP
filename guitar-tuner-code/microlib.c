
#include "microlib.h"

void init_ADC(void) {
    AD1PCFG = 0x7FFF;    // all PORTB = Digital but RB15 (AN15) = analog
    AD1CON1 = 0x00E0;    // SSRC bits = 111 implies internal counter ends sampling and starts converting
    AD1CON2 = 0;
    AD1CON3 = 0x1F02;    // Sample time = 31 TPB, Convertion time = 12*TAD = 12*[2*(ADCS+1)] TPB = 12*6 TPB
    AD1CSSL = 0;
    AD1CHS = 0x000F0000; // Connect RB15/AN15 as CH0 input
    AD1CON1SET = 0x8000; // turn ADC ON
}


void acquire_ADC(void) {
    int v;

    AD1CON1SET = (1<<SAMP);         // start Sampling (SAMP = 1) and then start converting
    while (!(AD1CON1 & (1<<DONE))); // conversion done?
    v = ADC1BUF0;                   // yes then get ADC value

    data[0] = (v >> 8) & 0x000000FF;
    data[1] =  v       & 0x000000FF;
}

void init_uart(void) {
    U1BRG = FPB/(4.0*BAUDRATE)-1;
    U1MODE =
        (1<<ON) |       // UART1 enabled
        (1<<BRGH) |     // baudrate generation with equation 21-2
        (0b00<<PDSEL) | // 8 databits, no parity (default)
        (0<<STSEL);     // 1 stopbit (default)
    U1STA =
        (1<<UTXEN) |    // transmission enabled
        (1<<URXEN);     // reception enabled
}

void myputchar(char c) {
    // Wait while TX buffer is full
    while (U1STAbits.UTXBF);

    // Transmit data
    U1TXREG = c;
}

char mygetchar(void) {
    // Wait until a byte is available
    while (!U1STAbits.URXDA);

    // Return received data
    return U1RXREG;
}

// Envia pela porta série a string apontada por p
void write_str(char *p) {
    char c;

    while ((c = *p++) != '\0')
        myputchar(c);
}

// Envia pela porta série em decimal, do valor contido em  b
void write_dec(uint8_t b) {
    uint8_t t;

    t = b % 100;
    myputchar('0' + b / 100);
    myputchar('0' + t / 10);
    myputchar('0' + t%10);
}

uint8_t hex2ascii(uint8_t b) {
    return((b > 9) ? 'A' + b - 10 : '0' + b);
}

// Envia pela porta série em hexadecimal, do valor contido em  b
void write_hex(uint8_t b) {
    myputchar(hex2ascii(b >> 4));
    myputchar(hex2ascii(b & 0x0F));
}

uint8_t hex2byte(char c) {
    return ((c < 'A') ? (c - '0') : ((c & ~0x20) - 'A' + 10));
}


void print_data(uint8_t base) {
    uint8_t n;

    for (n=0; n<8; n++) {
        switch (base) {
            case PRINT_HEX:
                write_hex(data[n]);
                break;
            case PRINT_DEC:
            default:
                write_dec(data[n]);
                break;
        }
        myputchar(' ');
    }
    myputchar('\n');
    myputchar('\r');
}


void __ISR(_TIMER_1_VECTOR, ipl2) Timer1Handler(void) {
    int static c = 0;

    if (++c == 10) {
        c = 0;
        if (led_int)
            LATFSET = 0x0001; // liga o LED
    }
    if (c >= led_int)
        LATFCLR = 0x0001; // desliga o LED

    mT1ClearIntFlag();    // clear the interrupt flag
}

void __ISR(_TIMER_2_VECTOR, ipl3) Timer2Handler(void) {
    // gera sinal sincronizado com o timer 2
    LATEINV = 0x0001;

    mT2ClearIntFlag(); // clear the interrupt flag
}

void set_motor(char signal, char n1, char n2){
    int graus = (n1-'0')*10 + (n2-'0');
    //float razao = graus / 90;
   // float t,temp;
    switch (signal){
        case '+':
           // t = 1.5 + (0.5*razao);
            OC1RS = (graus * 2500)/180 +3750;
            break;
        case '-':
             //t = 1.5 - (0.5*razao);
             OC1RS = (graus*-2500)/180 +3750;
            break;
    }
    //OC1RS = (t/50000*20);

}

/*int main(void) {
    char cmd[CMD_SIZE];
    uint8_t index = 0;
    char c;

    // Initialise UART
    init_uart();

    init_ADC();

    // Send string
    write_str("myprint demo\n\r");

    TRISFCLR = 0x0001;    // pino RF0 como saida

    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2 | T1_INT_SUB_PRIOR_0);
    INTEnableSystemMultiVectoredInt();
    OpenTimer1(T1_ON | T1_PS_1_8, 10000-1); // 1ms

    TRISECLR = 0x0001; // pino RE0 como saida
    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_3);
    INTEnableSystemMultiVectoredInt();
    OpenTimer2(T2_ON | T2_PS_1_32, 50000-1); // 1ms


    OC1CON = 0x0006;    // PWM mode on OCx; Fault pin disabled
    OC1RS = 5000;       // duty cycle = 50%
    OC1CONSET = 0x8000; // activa OC1

    // loop forever
    while(1) {
        c = mygetchar();
        if (c == 13) {
            switch (cmd[0]) {
                case 'h':
                    print_data(PRINT_HEX);
                    break;
                case 'd':
                    print_data(PRINT_DEC);
                    break;
                case 'w':
                    data[cmd[1]-'0'] = (hex2byte(cmd[2])<<4) + hex2byte(cmd[3]);
                    print_data(PRINT_HEX);
                    break;
                case 'a':
                    acquire_ADC();
                    print_data(PRINT_HEX);
                    break;
                case 'l':
                    led_int = hex2byte(cmd[1]);
                    OC1RS = led_int * 1000;
                    print_data(PRINT_HEX);
                    break;
                case 'm':
                    set_motor(cmd[1],cmd[2],cmd[3]);
                    print_data(PRINT_HEX);
                    break;
            }
            index = 0;
        }
        else
            if (index < CMD_SIZE)
                cmd[index++] = c;
    }
}*/
