#include <msp430.h>

unsigned int digits[10] = { 0xdb, 0x50, 0x1f, 0x5d, 0xd4, 0xcd, 0xcf, 0xd8, 0xdf, 
0xdd }; 

unsigned int cnt = 0; 
unsigned long usec = 0; 
unsigned int data = 0;
unsigned long start_time = 0; 
unsigned long end_time = 0; 
unsigned long delta_time = 0; 
unsigned long distance= 0; 
unsigned int step = 0; 
unsigned int menu = 0;
unsigned int dat = 0;
unsigned int dataa[4] = {0, }; 
unsigned int key = 0;
unsigned int men = 0; 
unsigned int birthday_data = 0; 
unsigned int flag = 0; 
unsigned int i = 0; 

void main(void) 
{ 
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
    
    P1DIR |= BIT0; 
    P4DIR |= BIT7; // 1.0 4.7. output
    
    P1OUT &= ~BIT0;
    P4OUT &= ~BIT7; // 1.0 4.7 off start
    
    P2REN |= BIT1; 
    P2OUT |= BIT1; 
    P2IE |= BIT1; 
    P2IES |= BIT1; 
    P2IFG &= ~BIT1; //2.1 interrupt
    
    P1REN |= BIT1; 
    P1OUT |= BIT1; 
    P1IE |= BIT1; 
    P1IES |= BIT1; 
    P1IFG &= ~BIT1; //1.1 interrupt
    
    P2DIR |= (BIT5 | BIT4); 
    P2SEL |= (BIT5 | BIT4); // motor
    
    //// keypad 
    // output 
    P2DIR |= (BIT0 | BIT2 | BIT3); 
    P2OUT |= (BIT0 | BIT2 | BIT3); // all high 
    // input 
    P6REN |= (BIT3 | BIT4 | BIT5 | BIT6); 
    P6OUT |= (BIT3 | BIT4 | BIT5 | BIT6); // pull up
    
    TA2CTL = TASSEL_2 + MC_1; // SMCLK : 1Mhz / Up mode to CCR0 
    TA2CCR0 = 1000; //PWM, Top in Up mode 
    TA2CCTL2 = OUTMOD_6; //PWM toggle/set 
    TA2CCR2 = 0; //left speed 
    TA2CCTL1 = OUTMOD_6; //PWM toggle/set 
    TA2CCR1 = 0; //right speed 

    // Digital Output - 7 segment 
    P3DIR |= 0xffff; 
    P3OUT &= 0x0000; 
    P4DIR |= 0x000f; 
    P4OUT &= ~BIT0; 
    
    // ultrasonic 
    P2DIR |= BIT7; 
    P2OUT &= ~BIT7; // trig 
    P1IE |= BIT4; 
    P1IES &= ~BIT4; // rising edge //P1IES |= BIT4; // falling edge 
    P1IFG &= ~BIT4; 
    
    // Timer - Timer0 
    TA0CCTL0 = CCIE; 
    TA0CCR0 = 5000; //1000; 
    TA0CTL = TASSEL_2 + MC_1 + TACLR; // SMCLK : 1Mhz / Up mode to CCR0 
    
    // Timer - Timer1 
    TA1CCTL0 = CCIE; 
    TA1CCR0 = 50; //50us 
    TA1CTL = TASSEL_2 + MC_1 + TACLR; // SMCLK : 1Mhz / Up mode to CCR0 
    
    P6SEL |= BIT0; 
    ADC12CTL0 = ADC12SHT02 + ADC12ON; 
    ADC12CTL1 = ADC12SHP; 
    ADC12MCTL0 = ADC12INCH_0; 
    ADC12CTL0 |= ADC12ENC; //ADC set

    __bis_SR_register(GIE);

    while(1) 
    { 
        // columns 1 
        P2OUT &= ~BIT2; 
        P2OUT |= (BIT0 | BIT3);
        
        if((P6IN & BIT3) == 0) 
        { 
            key = 1; 
        } 
        else if((P6IN & BIT6) == 0) 
        { 
            key = 4; 
        } 
        else if((P6IN & BIT5) == 0) 
        { 
            key = 7; 
        } 
        else if((P6IN & BIT4) == 0) // * 
        { 
            while((P6IN & BIT4) == 0) 
            key = 0;//key = 10;
            men++; 
        }
        
        // columns 2 
        P2OUT &= ~BIT0; 
        P2OUT |= (BIT2 | BIT3);
        
        if((P6IN & BIT3) == 0) 
        {
            key = 2; 
        } 
        else if((P6IN & BIT6) == 0) 
        { 
            key = 5; 
        } 
        else if((P6IN & BIT5) == 0) 
        { 
            key = 8; 
        } 
        else if((P6IN & BIT4) == 0) 
        { 
            key = 0; 
        }
        
        // columns 3 
        P2OUT &= ~BIT3; 
        P2OUT |= (BIT0 | BIT2); 
        
        if((P6IN & BIT3) == 0) 
        { 
            key = 3; 
        } 
        else if((P6IN & BIT6) == 0) 
        { 
            key = 6; 
        } 
        else if((P6IN & BIT5) == 0) 
        { 
            key = 9; 
        } 
        else if((P6IN & BIT4) == 0) // # 
        { 
            //key= 11; 
            men = 0; 
            for(i=0 ; i<4 ; i++) 
            { 
                dataa[i] = 0; 
            } 
        }
        
        // save key_number 
        if(men == 0) 
        { 
            dataa[3] = key; 
        } 
        else if(men == 1) 
        { 
            dataa[2] = key; 
        } 
        else if(men == 2) 
        { 
            dataa[1] = key; 
        } 
        else if(men == 3) 
        {
            dataa[0] = key; 
        } 
        else if(men == 4) 
        { 
            birthday_data = dataa[3]*1000 + dataa[2]*100 + dataa[1]*10 + dataa[0];
        
            if(birthday_data != 704)
            {
                men = 0; 
                for(i=0 ; i<4 ; i++) 
                { 
                    dataa[i] = 0; 
                }
            }
            
            else
            {
                while(1)
                {
                    ADC12CTL0 |= ADC12SC; 
                    while (!(ADC12IFG & BIT0)); 
                    dat = ADC12MEM0;
                    
                    if(flag == 0) 
                    { 
                        P2OUT |= BIT7; // trig on 
                        __delay_cycles(10); //10us 
                        P2OUT &= ~BIT7; // trig off 
                        flag = 1; 
                    }
                    
                    if(dat >= 2000)
                    {
                        if(data <= 30) 
                        { 
                            TA2CCR2 = 0; 
                            TA2CCR1 = 0; 
                            P1OUT &= ~BIT0; 
                            P4OUT &= ~BIT7;
                        } 
                        else 
                        { 
                            if(step % 3 == 0 && menu == 0) 
                            { 
                                TA2CCR2 = 0; 
                                TA2CCR1 = 0; 
                                P1OUT &= ~BIT0; 
                                P4OUT &= ~BIT7; 
                            } 
                            else if(step % 3 == 1 && menu == 0) 
                            { 
                                TA2CCR2 = 0; 
                                TA2CCR1 = 450; 
                                P4OUT |= BIT7; 
                                P1OUT &= ~BIT0;
                            } 
                            else if(step % 3 == 2 && menu == 0) 
                            { 
                                TA2CCR2 = 0; 
                                TA2CCR1 = 850; 
                                P4OUT |= BIT7; 
                                P1OUT &= ~BIT0;
                            } 
                            else if(menu % 3 == 0 && step == 0) 
                            { 
                                TA2CCR2 = 0; 
                                TA2CCR1 = 0; 
                                P1OUT &= ~BIT0; 
                                P4OUT &= ~BIT7; 
                            } 
                            else if(menu % 3 == 1 && step == 0) 
                            { 
                                TA2CCR2 = 450; 
                                TA2CCR1 = 0; 
                                P1OUT |= BIT0; 
                                P4OUT &= ~BIT7;
                            } 
                            else if(menu % 3 == 2 && step == 0) 
                            { 
                                TA2CCR2 = 850; 
                                TA2CCR1 = 0; 
                                P1OUT |= BIT0;
                                P4OUT &= ~BIT7;
                            }
                        }
                    }  
                    
                    if(dat < 2000)
                    {
                        if(data <= 15) 
                        { 
                            TA2CCR2 = 0; 
                            TA2CCR1 = 0; 
                            P1OUT &= ~BIT0; 
                            P4OUT &= ~BIT7;
                        } 
                        else 
                        { 
                            if(step % 3 == 0 && menu == 0) 
                            { 
                                TA2CCR2 = 0; 
                                TA2CCR1 = 0; 
                                P1OUT &= ~BIT0; 
                                P4OUT &= ~BIT7; 
                            } 
                            else if(step % 3 == 1 && menu == 0) 
                            { 
                                TA2CCR2 = 0; 
                                TA2CCR1 = 450; 
                                P4OUT |= BIT7; 
                                P1OUT &= ~BIT0;
                            } 
                            else if(step % 3 == 2 && menu == 0) 
                            { 
                                TA2CCR2 = 0; 
                                TA2CCR1 = 850; 
                                P4OUT |= BIT7; 
                                P1OUT &= ~BIT0;
                            } 
                            else if(menu % 3 == 0 && step == 0) 
                            { 
                                TA2CCR2 = 0; 
                                TA2CCR1 = 0; 
                                P1OUT &= ~BIT0; 
                                P4OUT &= ~BIT7; 
                            } 
                            else if(menu % 3 == 1 && step == 0) 
                            { 
                                TA2CCR2 = 450; 
                                TA2CCR1 = 0; 
                                P1OUT |= BIT0; 
                                P4OUT &= ~BIT7;
                            } 
                            else if(menu % 3 == 2 && step == 0) 
                            { 
                                TA2CCR2 = 850; 
                                TA2CCR1 = 0; 
                                P1OUT |= BIT0;
                                P4OUT &= ~BIT7;
                            } 
                        }    
                    }
                }
            }
        }
    } 
} 
 
#pragma vector=PORT1_VECTOR 
__interrupt void Port_1(void) 
{ 
    
    if(P1IFG & BIT1)
    {
        if(menu == 0)
        {
            step++;
        }
        else
        {
           step=0;
           menu=0;
        }
    }
    P1IFG &= ~BIT1;
    
    if(P1IFG & BIT4) 
    {   
        P1DIR &= ~BIT4;
        if((P1IES & BIT4) == 0) // if rising edge 
        { 
            usec = 0; 
            //TA1CTL |= TACLR; 
            P1IES |= BIT4; // falling edge 
        } 
        else if(P1IES & BIT4) // if falling edge 
        { 
            if(usec > 3 && usec < 500) // 150us ~ 25ms 
            {
                data = usec*50/58; //TA1R; 
            }
            else if(usec >= 760) 
            {
                data = 9999; 
            }
            else 
            {
                data = 0; 
            }
            P1IES &= ~BIT4; // rising edge 
            //flag = 0; 
        } 
    } 
    P1IFG &= ~BIT4; //IFG is cleared 
    
}

#pragma vector=PORT2_VECTOR 
__interrupt void Port_2(void) 
{ 
    if(step == 0)
    {
        menu++;
    }
    else
    {
        step=0;
        menu=0;
    }
    P2IFG &= ~BIT1;
} 

// Timer interrupt service routine 
#pragma vector=TIMER0_A0_VECTOR 
__interrupt void TIMER0_A0_ISR(void) 
{   
    if(men <= 3)
    {
        cnt++; 
        if(cnt > 3) 
            cnt = 0; 
        switch (cnt) 
        { 
            case 0: 
                P3OUT = digits[dataa[0]]; 
                P4OUT = (~BIT0 & 0xf);
                break; 
            case 1: 
                P3OUT = digits[dataa[1]]; 
                P4OUT = (~BIT1 & 0xf);
                break; 
            case 2: 
                P3OUT = digits[dataa[2]]; 
                P4OUT = (~BIT2 & 0xf);
                break; 
            case 3: 
                P3OUT = digits[dataa[3]]; 
                P4OUT = (~BIT3 & 0xf);
                break; 
        } 
    }
    
    else
    {
        cnt++; 
        if(cnt > 3) 
            cnt = 0; 
        switch (cnt) 
        { 
            case 0: 
                P3OUT = digits[data%10]; 
                P4OUT = (~BIT0 & 0xf);
                break; 
            case 1: 
                P3OUT = digits[data/10%10]; 
                P4OUT = (~BIT1 & 0xf);
                break; 
            case 2: 
                P3OUT = digits[data/100%10]; 
                P4OUT = (~BIT2 & 0xf);
                break; 
            case 3: 
                P3OUT = digits[data/1000%10]; 
                P4OUT = (~BIT3 & 0xf);
                break; 
        }
    }
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void) 
{ 
    usec++; 
    
    if(flag == 1 && usec > 1000) 
    { 
        flag = 0; 
    } 
}