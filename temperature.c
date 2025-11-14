#include <xc.h>
#define _XTAL_FREQ 8000000                     // Set at 8 MHz

// CONFIG1
#pragma config FOSC  = INTRC_NOCLKOUT         // High speed internal oscillator
#pragma config WDTE  = OFF
#pragma config PWRTE = OFF
#pragma config MCLRE = OFF
#pragma config CP    = OFF
#pragma config CPD   = OFF
#pragma config BOREN = OFF
#pragma config IESO  = ON
#pragma config FCMEN = ON
#pragma config LVP   = OFF

// CONFIG2
#pragma config BOR4V = BOR40V
#pragma config WRT   = OFF

/*
 * Temperature conversion equation used:
 *
 *   ADC_value = (ADRESH << 8) | ADRESL
 *   Voltage   = ADC_value * (5.0 / 1023.0)
 *   Temp_C    = Voltage / 0.01
 *
 * Explanation:
 *   - ADC range: 0–1023 (10-bit)
 *   - Vref = 5V
 *   - LM35 output: 10 mV/°C (0.01 V/°C)
 *   - Therefore: Temp_C = Voltage / 0.01
 */

float tempC;
unsigned int result;
unsigned int whole;
unsigned int decimal;

/*
 * Negative temperature support (future implementation):
 * LM35 produces negative voltage for T < 0°C.
 * PIC ADC cannot read negative voltages.
 * These variables remain commented until hardware offset is added.
 */
/*
signed int result_negative;
signed int whole_negative;
signed int decimal_negative;
*/

unsigned char display_digits_decimal[10] = 
{
    0x3F,
    0x06,
    0x5B,
    0x4F,
    0x66,
    0x6D,
    0x7D,
    0x07,
    0x7F,
    0x6F
};

unsigned char display_digits_units[10] = 
{
    0x3F,
    0x06,
    0x5B,
    0x4F,
    0x66,
    0x6D,
    0x7D,
    0x07,
    0x7F,
    0x6F
};

unsigned char display_digits_tens[10] = 
{
    0x3F,
    0x06,
    0x5B,
    0x4F,
    0x66,
    0x6D,
    0x7D,
    0x07,
    0x7F,
    0x6F
};

unsigned char ERR[3] = {
    0x79, // E
    0x31, // r
    0x31  // r
};

void setRegisters()
{
    OSCCON = 0b01110000;        // Set at 8 MHz
    while (OSCCONbits.HTS == 0);
    TRISAbits.TRISA0 = 1;
    ANSELbits.ANS0 = 1;
    ANSELbits.ANS5 = 0;
    ANSELH = 0x00;

    TRISB = 0x00;
    TRISC = 0x00;
    TRISD = 0x00;
    TRISEbits.TRISE0 = 0;
}

void initRegisters()
{
    ADCON0bits.ADCS1 = 1;
    ADCON0bits.ADCS0 = 1;
    ADCON0bits.CHS3 = 0;
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS0 = 0;
    ADCON0bits.ADON = 1;

    ADCON1bits.ADFM = 1;
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.VCFG0 = 0;

    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTEbits.RE0 = 1;
}

void main(void)
{
    setRegisters();
    initRegisters();

    while (1)
    {
        __delay_ms(10);
        ADCON0bits.GO_DONE = 1;
        while (ADCON0bits.GO_DONE);

        result = (ADRESH << 8) | ADRESL;
        // result_negative = (ADRESH << 8) | ADRESL;   // Disabled until negative support is added

        /*
         * Temperature calculation based on equation above
         */
        tempC = (result * 5.0 / 1023.0) / 0.01;

        if(tempC < 100 && tempC >= 0)
        {
            whole   = (unsigned int)tempC;
            decimal = (unsigned int)(tempC * 10) % 10;

            unsigned char tens_digit = whole / 10;
            unsigned char units_digit = whole - (tens_digit * 10);

            PORTB = display_digits_tens[tens_digit];
            PORTC = display_digits_units[units_digit];
            PORTD = display_digits_decimal[decimal];
            PORTEbits.RE0 = 1;
        }

        if(tempC >= 100)
        {
            PORTEbits.RE0 = 0;
            PORTB = ERR[0];
            PORTC = ERR[1];
            PORTD = ERR[2];
            __delay_ms(500);
            PORTB = 0x00;
            PORTC = 0x00;
            PORTD = 0x00;
            __delay_ms(500);
        }

        /*
         * Future negative temperature handling
         * Requires hardware offset because LM35 outputs negative voltage for T < 0°C.
         */
        /*
        if(tempC < 0.0)
        {
            whole_negative   = (signed int)tempC;
            decimal_negative = (signed int)(tempC * 10) % 10;

            signed char tens_digit = whole_negative / 10;
            signed char units_digit = whole_negative - (tens_digit * 10);

            PORTB = 0x40; // '-' sign
            PORTC = display_digits_units[units_digit];
            PORTEbits.RE0 = 1;
            PORTD = display_digits_decimal[decimal_negative];
        }
        */
    }
}