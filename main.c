/*
 * File:   democlass.c
 * Author: pfdlo
 *  
 * Created on 21 February 2017, 14:26
 */

#define FCY 29491200L
 



/* Include List */
#include <libpic30.h>
#include <p30F4011.h>
#include <stdio.h>
#include <adc10.h>



/* Global Definitions */

/* For XC16 Ver <= 1.24 */
//_FOSC(CSW_FSCM_OFF & XT_PLL16); //oscillator at 16x crystal frequency
//_FWDT(WDT_OFF);                 //watchdog timer off
//_FBORPOR(MCLR_EN & PWRT_OFF);   //Turn MCLR pin ON and Energy Timers OFF

/* For XC16 Ver >= 1.24 */
#pragma config FCKSMEN=CSW_FSCM_OFF
#pragma config FOS=PRI
#pragma config FPR=XT_PLL16 
#pragma config WDT=WDT_OFF
#pragma config MCLRE=MCLR_EN
#pragma config FPWRT=PWRT_OFF

/* Global Variables */
unsigned char tecla = '\0';
unsigned int Channel, PinConfig, Scanselect;
unsigned int Adcon3_reg, Adcon2_reg, Adcon1_reg;

 int goADC=0;

/*UART 2 Interruption Cycle*/
void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void){
    
    IFS1bits.U2RXIF = 0;
    while(U2STAbits.URXDA){
        tecla = U2RXREG;
        }      
}   

void __attribute__((interrupt, auto_psv)) _T2Interrupt(void)
{		
    IFS0bits.T2IF = 0; 
    goADC=1;
       
}


/* declaration o function*/
void config_uart(void);
void config_adc(void);
void delay_ms(unsigned int delay); //delay in miliseconds

//REMEBER:
//Set pin signal state using LAT (ex. for pin RE8 LOW: _LATE8 = 0;)
//Set pin as Input or Output using TRIS (ex. for pin RE8 as output: _TRISE8 = 0;)
//Read pin digital value using PORT (ex. assign pin RE8 value to variable: valor = _RE8;)
//(Refer to Family Ref. Manual, Section 11. I/O Ports, page 253)

/*Main Function*/
int main(void) {
    
    //user variables
    int val = 0;
    float voltage = 0;
    
    _TRISF0 = 0;
    _LATF0 = 0;
    
    _TRISC13 = 0;
    _LATC13 = 1;
    
    /*Timer 2 1ms/1kHz*/
    T2CONbits.TON = 0;      //Timer_2 is OFF
    TMR2 = 0;               //resets Timer_2
    PR2 = 29491;            //sets the maximum count for Timer_2
    T2CONbits.TCS = 0;      //choose FCY as clock source for Timer_2
    T2CONbits.TCKPS = 0x00; //sets the Timer_2 pre-scaler to 1
    IFS0bits.T2IF = 0;      //clears Timer_2 interrupt flag
    _T2IE = 1;              //enables Timer_2 Interrupts
    T2CONbits.TON = 1;      //turns Timer_2 OFF     
    
    /*PWM configure for OC2 - 1kHz@50% */
    OC2R = 0;               //Initial Delay (only for the first cycle)
    OC2RS = PR2/2;          //sets the initial duty_cycle;
    OC2CONbits.OCM = 6;     //set OC2 mode to PWM
    OC2CONbits.OCTSEL = 0;  //selects Timer_2 as the OC2 clock source
   
/***********************
* Serial Port configuration bits - UART2
***********************/
    U2MODEbits.STSEL = 0;   //one stop bit
    U2MODEbits.PDSEL = 0;   //data length and parity: 8 bits, no parity
    U2BRG = 15;             //set baud rate 115200
    U2MODEbits.UARTEN = 1;  //enable uart 2
    U2STAbits.UTXEN = 1;	//enable uart 2 tx
    U2STAbits.URXISEL = 0;  //uart2 receive interrupt at each received character
    U2STAbits.UTXISEL = 0;  //uart2 send interrupt at each transmited character
    IFS1bits.U2RXIF = 0;    //uart2 receive interrupt clear flag
    IFS1bits.U2TXIF = 0;    //uart2 send interrupt clear flag
    IEC1bits.U2TXIE = 0;    //enable uart2 transmit interrupts
    IEC1bits.U2RXIE = 1;    //enable uart2 receive interrupts

    __C30_UART=2;           //point printf to UART2 (only needed if use printf)
/***********************/
    
    T2CONbits.TON = 1;      //Turn T2 ON!!!
    
    
    printf("\n\rSerial port ONLINE \n"); 
    ADPCFG = 0xFC;			//AN0 and AN1 are analog inputs //	ADPCFG = 0xFFFB; // all PORTB = Digital; RB2 = analog
    
    
    
    
    while(1)
    {
        if(goADC){     
            config_adc();
            val = convert_adc();
            voltage = val*0.00488-0.1;
            printf("Converte ADC = %d, Voltage: %f V \r\n", val,voltage);
            //__delay_ms(250);
            _LATF0 = ~_LATF0 ; // Toglle LED
            goADC = 0;
        }
    }
return 0;
}

void config_adc(void){
	//Configure ADC
	ADCON1bits.ADON = 0; 	//turns the ADC module OFF
	ADCON1bits.FORM = 0;	//result as an integer
	ADCON1bits.SSRC = 0;	//clearing sampling bit starts convertion
	ADCON1bits.SIMSAM = 0;	//samples each channel at a time
	ADCON1bits.ASAM = 0;	//sampling begins when samp bit is set
//ADCON1 = 0;
	ADCON2bits.VCFG = 0;	//VrefH+ = AVcc and Vref- = AGND
	ADCON2bits.CHPS = 0;	//samples CH0 only
	ADCON2bits.SMPI = 0;	//interrups at every sample sequence
	ADCON2bits.BUFM = 0;	//result buffer used as a 16bit word
	ADCON2bits.ALTS = 0;	//always use MUX-A
	ADCON2bits.CSCNA = 1;	//enable scan
//ADCON2 = 0x0400;

	ADCON3bits.SAMC = 0xF;	//16 Tad time adc 
	ADCON3bits.ADRC = 0;	//use system clock for convertion
	ADCON3bits.ADCS = 0xF;	//16 Tcy(cpu cycles) for convertion time
//ADCON2 = 0x0F0F;
	

	ADCHSbits.CH123NB = 0;	//negative input is Vref-
	ADCHSbits.CH123SB = 0;	//positive input is AN0 and so on
	ADCHSbits.CH0NB = 0;	//negative input is Vref-
	ADCHSbits.CH0SB = 0;	//positive input is AN0 and so on
	ADCHSbits.CH123NA = 0;	//negative input is Vref-
	ADCHSbits.CH123SA = 0;	//positive input is AN0 and so on
	ADCHSbits.CH0NA = 0;	//negative input is Vref-
	ADCHSbits.CH0SA = 0;	//positive input is AN0 and so on
	
//	ADPCFG = 0xFC;			//AN0 and AN1 are analog inputs

	ADCSSL = 0x01;			//AN0 and AN1 are enabled for scan

	ADCON1bits.ADON = 1; 	//turns the ADC module ON

}
int convert_adc(){
	int ADCValue1 = 0;
	int ADCValue2 = 0;
	
	ADCSSL = 0x01;			//AN0 and AN1 are enabled for scan
	
	ADCON1bits.SAMP = 1;		//start sampling
	delay_ms(50);				//delay 50ms for sampling
	ADCON1bits.SAMP = 0;		//start converting
	while (!ADCON1bits.DONE); 	// conversion done?
	ADCValue1 = ADCBUF0; 		// yes then get ADC value
	
	ADCSSL = 0x02;			//AN0 and AN1 are enabled for scan
	
	ADCON1bits.SAMP = 1;		//start sampling
	delay_ms(50);				//delay 50ms for sampling
	ADCON1bits.SAMP = 0;		//start converting
	while (!ADCON1bits.DONE); 	// conversion done?
	ADCValue2 = ADCBUF0; 		// yes then get ADC value
	
	//printf("V=%d %d \n\r" , ADCValue1, ADCValue2);
	return ADCValue1;
}

void delay_ms(unsigned int delay) //delay in miliseconds
{   
 	unsigned int cycles;	// number of cycles
 	for(;delay;delay--)		
    for(cycles=FCY;cycles;cycles--); //~1ms cycle
}