
#include "stm32f1xx.h"
#include "main.h"

int overvoltagecounter=0  ;  // count up each 25ms
int overcurrentcounter=0  ;  // count up each 200us
int counter=0  ;   // count up each 50us
int tempcounter=0  ;   //    count up each second under a specified condition
int onesecondcounter=0    ;  // count up each 25ms to create one second (40*25ms)
int countertwo=0 ;
unsigned char uartpacket[8] ;
int packetcounter=0  ;  //   count up to define which uart packet should be sent
int error ;
int phasecounterpeak ;
int integrator=0 ;
int D ;
int phasecounter=0 ;
int phase;
int frequency=35;
int sintheta;
int voutsense;
int vlinkdc;
int ilinkdc;
int heatsinktemperature;
int invfrequency=20000 ;   //  in Hertz
int sinlookuptable[361] = {0, 18, 36, 54, 71, 89, 107, 125, 142, 160, 178, 195, 213, 230, 248, 265, 282, 299, 316, 333, 350, 367, 384, 400, 416, 433, 449, 465, 481, 496, 512, 527, 543, 558, 573, 587, 602, 616, 630, 644, 658, 672, 685, 698, 711, 724, 737, 749, 761, 773, 784, 796, 807, 818, 828, 839, 849, 859, 868, 878, 887, 896, 904, 912, 920, 928, 935, 943, 949, 956, 962, 968, 974, 979, 984, 989, 994, 998, 1002, 1005, 1008, 1011, 1014, 1016, 1018, 1020, 1021, 1023, 1023, 1024, 1024, 1024, 1023, 1023, 1022, 1020, 1018, 1016, 1014, 1011, 1008, 1005, 1002, 998, 994, 989, 984, 979, 974, 968, 962, 956, 949, 943, 935, 928, 920, 912, 904, 896, 887, 878, 868, 858, 849, 839, 828, 818, 807, 796, 784, 773, 761, 749, 737, 724, 711, 698, 685, 672, 658, 644, 630, 616, 602, 587, 573, 558, 543, 527, 512, 496, 481, 465, 449, 433, 417, 400, 384, 367, 350, 333, 316, 299, 282, 265, 248, 230, 213, 195, 178, 160, 143, 125, 107, 89, 71, 54, 36, 18, 0, -18, -36, -54, -71, -89, -107, -125, -143, -160, -178, -195, -213, -230, -248, -265, -282, -299, -316, -333, -350, -367, -384, -400, -417, -433, -449, -465, -481, -497, -512, -527, -543, -558, -573, -587, -602, -616, -630, -644, -658, -672, -685, -698, -711, -724, -737, -749, -761, -773, -784, -796, -807, -818, -828, -839, -849, -859, -868, -878, -887, -896, -904, -912, -920, -928, -935, -943, -949, -956, -962, -968, -974, -979, -984, -989, -994, -998, -1002, -1005, -1008, -1011, -1014, -1016, -1018, -1020, -1021, -1023, -1023, -1024, -1024, -1024, -1023, -1023, -1022, -1020, -1018, -1016, -1014, -1011, -1008, -1005, -1002, -998, -994, -989, -984, -979, -974, -968, -962, -956, -949, -943, -935, -928, -920, -912, -904, -896, -887, -878, -868, -859, -849, -839, -828, -818, -807, -796, -784, -773, -761, -749, -737, -724, -711, -698, -685, -672, -658, -644, -630, -616, -602, -587, -573, -558, -543, -527, -512, -496, -481, -465, -449, -433, -416, -400, -384, -367, -350, -333, -316, -299, -282, -265, -248, -230, -213, -195, -178, -160, -143, -125, -107, -89, -71, -54, -36, -18, 0};       // sintheta value * 1024
int main(void)
{

    	FLASHCONFIG() ;
    	CLOCK() ;
    	WATCHDOG() ;
    	POWER() ;
        GPIO()  ;
        PWM() ;
        TIM1Interrupt()  ;
        ADC()   ;
        UART()  ;


while(1)
{
	WWDG->CR = (1 << 6) | (1 << 7) | 63 ;    //  enable watchdog, set T6 to avoid immediate reset(read reference manual)

    if(countertwo >= 500)        //    the frequency is defined each 500*50us = 25ms
     	   {
         onesecondcounter=onesecondcounter+1  ;
         countertwo=0 ;

     if(vlinkdc > 3230)      //        dclink voltage value is more than approximately 350V
       {
      overvoltagecounter=overvoltagecounter+1 ;
       	if(overvoltagecounter >= 2)
        	{     overvoltagecounter=0   ;
           GPIOA->BSRR = (1 << 7);         //      shutdown the mosfet drivers after 25ms
           GPIOA->BSRR = (1 << 4);         //      turn on RED LED
           }
        }
         if(vlinkdc > 3090)             //        dclink voltage value is more than approximately 335V
          {
        	overvoltagecounter=overvoltagecounter+1 ;
              	if(overvoltagecounter >= 4)
              	{     overvoltagecounter=0   ;
              	GPIOA->BSRR = (1 << 7);         //      shutdown the mosfet drivers after 3*25ms
            	GPIOA->BSRR = (1 << 4);         //      turn on RED LED
            	}
           }

           if(vlinkdc < 3025)             //        dclink voltage value is less than approximately 325V
          	{

             	GPIOA->BSRR = (1 << 23);         //      deactivate shutdown pin of the mosfet drivers because protection is not necessary
            	GPIOA->BSRR = (1 << 20);         //      turn off RED LED
            }
           if(onesecondcounter >= 40)
               {
               onesecondcounter=0 ;
        	   uartpacket[0] = ADC2->JDR2 ;
               uartpacket[1] = ADC2->JDR2 >> 8 ;
               uartpacket[2] = ADC1->JDR2 ;
               uartpacket[3] = ADC1->JDR2 >> 8 ;
               uartpacket[4] = frequency*10 ;
               uartpacket[5] = (frequency*10) >> 8 ;
               uartpacket[6] = 45 ;
               uartpacket[7] = 45 ;
                packetcounter=0 ;
               if(heatsinktemperature > 2800)         //   temperature less than about 55C
               {
               GPIOC->BSRR = (1 << 29) ;    //     turn off Fan
                 }

                if(heatsinktemperature < 2700 && heatsinktemperature > 2500)           //   temperature less than about 75C and more than 60C
                {
                tempcounter=tempcounter+1 ;
                 if(tempcounter >= 16)
                 	{     tempcounter=0   ;
                  	GPIOC->BSRR = (1 << 13) ;    //    if temperature less than about 75C and more than 60C after 15 second turn on Fan
                	}
                  }

                  if(heatsinktemperature < 2350)           //   temperature more than 85C
                   {
                    tempcounter=tempcounter+1 ;
                     if(tempcounter >= 4)
                      	{
                    	 tempcounter=0   ;
                         GPIOA->BSRR = (1 << 7);         //     if temperature more than 85C after 3 second shutdown the mosfet drivers after 3 second
                         	}
                         }

                 if(heatsinktemperature > 2700)    //   temperature less than about 60C
                   {
                   GPIOA->BSRR = (1 << 23);         //      deactivate shutdown pin of the mosfet drivers because protection is not necessary

                    }
                     }
        	        }
                       if(packetcounter < 8)
                      {
                      	 USART3->DR=uartpacket[packetcounter]   ;     //  put data in usart3 to transmit
                         while(!(USART3->SR & 1 << 6)) ;        //   wait for transmission complete
                          packetcounter=packetcounter+1  ;

                        }
    if(counter >= 4)
    {
      counter=0 ;
        if(ilinkdc > 2457)               //        dclink overcurrent , dclink peak current value is more than 19.8A
             	{
      	 overcurrentcounter=overcurrentcounter+1  ;
         if(overcurrentcounter >= 2)
          {
          overcurrentcounter=0   ;
          GPIOA->BSRR = (1 << 7);         //      shutdown the mosfet drivers after 200us
          	GPIOA->BSRR = (1 << 2) ;    //     turn on YELLOW LED
           	 }
       	}

           if(ilinkdc > 2180)               //        dclink overcurrent , dclink peak current value is more than 17.5A peakcurrent
        	{
       	 overcurrentcounter=overcurrentcounter+1  ;
        	 if(overcurrentcounter >= 4)
                 {
          	 overcurrentcounter=0   ;
             	GPIOA->BSRR = (1 << 7);         //      shutdown the mosfet drivers after 3*200us
             	GPIOA->BSRR = (1 << 2) ;    //     turn on YELLOW LED
                 	}
              	}

                 if(ilinkdc < 2048)               //         dclink peak current value less than 16.5A
               	{

             GPIOA->BSRR = (1 << 23);         //      deactivate shutdown pin of the mosfet drivers because protection is not necessary
             GPIOA->BSRR = (1 << 18) ;    //     turn off YELLOW LED
            	}
               }
}

}


void TIM1_UP_IRQHandler(void)
{
	 TIM1->SR = 0 ;       //  clear Update interrupt flag
	 GPIOA->BSRR = (1 << 3);           //   trig on GREEN LED to define the runtime of program



    phasecounterpeak=invfrequency/frequency ;

    if(phasecounter >= phasecounterpeak)
    	     				{
    	     					phasecounter=0 ;
    	     				}
 	phasecounter=phasecounter+1 ;
 	countertwo=countertwo+1;
 	    counter=counter+1   ;
 	 ADC1->CR2 |= 1 << 21 ;       //    ADC1 software trigger
 	 ADC2->CR2 |= 1 << 21;         //    ADC2 software trigger
   voutsense = ADC1->JDR1-2048 ;
   frequency = ADC2->JDR1*50/4095 ;   //  in Hertz, based on the nominal frequency 50Hz

   ilinkdc = ADC1->JDR2 ;      //   assume when ilinkdc is 33A, ADC pin is 3.3V
   vlinkdc = ADC2->JDR2 ;    //   assume when vlinkdc is 300*sqrt(2) Volt, ADC pin is 3.3V

   heatsinktemperature= ADC2->JDR3 ;       //   in ADC value format



  		       phase=phasecounter*360*frequency/invfrequency  ;
  		       sintheta=sinlookuptable[phase] ;              //   create sin wave reference via phasecounter
  		     error=frequency*sintheta/34-voutsense ;      //  apply V/f method  , 1/34 = 2048/(68*1024) , 300/68.18 = 220/50
               integrator=integrator + error >> 1  ;        //   PI controller , Ki = 0.5
            			   D = 35*error/10+integrator ;               //   create duty cycle  ,     //   PI Controller proportional coefficient Kp = 3.5

   if (D > 1659)
   {
	   D=1659 ;
   }
   if (D < -1659)
   {
	   D=-1659 ;
   }
   if (D >= 0)                    //     apply unipolar SPWM
   {
   TIM1->CCR1 = D  ;
   TIM1->CCR2 = 0  ;
   }
   if (D < 0)                      //     apply unipolar SPWM
   {
	TIM1->CCR1 = 0  ;
   TIM1->CCR2 = -D  ;
   }



   GPIOA->BSRR = (1 << 19);           //   trig off GREEN LED

}

void FLASHCONFIG(void)
{
	FLASH->ACR = 2 | 1 << 4 ;        //      prefetch buffer enable (to increase performance), enable 2 wait states due to the sysclk frequency between 48MHz and 72MHz
    while(!(FLASH->ACR &  1 << 5)) ;         // prefetch buffer enable ready flag


}

void CLOCK(void)
{


	    RCC->CFGR = (7 << 18) | (0 << 17) | (1 << 16) | (2 << 14) | (4 << 8) | (2 << 0) ;   // PLL multiplication factor = 9 , set pll entry clock source as HSE, set no divider for PLL entry, ADC prescaler = 6, APB1 prescaler = 2,  SYSCLK source = PLL
        RCC->CR = (1 << 16) | (1 << 24) | (0 << 19)    ;   // HSE CLOCK ENABLE, PLL CLOCK ENABLE, CLOCK SECURITY DISABLE

	    while(!(RCC->CFGR &  2)) ;  // SYSCLK source Status HSE
     	while(!(RCC->CR &  1 << 17)) ;         // HSE clock ready flag
    	while(!(RCC->CR &  1 << 25)) ;        // PLL clock ready flag

    	RCC->APB2ENR = (1 << 0) | (15 << 2) | (1 << 11) | (1 << 9) | (1 << 10);   // Alternate function IO clock enable, IO port A,B,C,D clock enable, ADC1 and ADC2 and advanced TIMER1 clock Enable
        RCC->APB1ENR = (1 << 11) | (1 << 18) | (1 << 28)  ;  // Window Watchdog enable, USART3 Clock Enable and Power interface clock enable

}

void WATCHDOG(void)
{
	WWDG->CFR = 127 | (3 << 7) ;  //  set Window Watchdog=127 (64+0) , set prescaler= 1/(8*4096)
	WWDG->CR = (1 << 6) | 63 ;  //  set T6 to prevent immediate reset and set watchdog downcounter to T=127 (64+63) , reset period ~ 63ms

}



	void POWER(void)
	{
	        PWR->CR = (9 << 4) ;    //PVD ENABLE - PVD LEVEL is 2.6V
	        while(PWR->CSR & 1 << 2) ;   //   wait if power voltage lower than 2.6V

	}



void GPIO(void)
{
	  GPIOA->CRL = 0x10011100    ;   //  Config ADC pins, three LED pins, SD pin (To shut down the gate drivers)
      GPIOA->CRH = 0x88888899    ;   //  Config Timer1 ch1 ch2 pins
      GPIOB->CRL = 0x88888880    ;   //  Config ADC pin
      GPIOB->CRH = 0x89988988    ;   //  Config USART3, Timer ch1n ch2n


      GPIOC->CRH = 0x88144444 ;      // Config PC13 as Fan enable
      GPIOD->CRL = 0x44444488 ;     //  Config OSC pins


      GPIOA->ODR = 0xFC00     ;     // Pull up unused pins along with main inputs
	  GPIOB->ODR = 0x9BFE     ;     // Pull up unused pins along with main inputs
	  GPIOC->ODR = 0xC000     ;    //  Pull up unused pins
	  GPIOC->ODR = 0x0002     ;    //  Pull up unused pins along with main inputs

}


void ADC(void)

{
   ADC1->CR2 = 1 ;
   ADC2->CR2 = 1 ;
   ADC1->CR2 |= 2 | 15 << 12 | 0 << 11  ;        //    enable CONT  , set right alignment, set JSWSTART as a external trigger and injected conversion on external trigger enable for ADC1
   ADC2->CR2 |= 2 | 15 << 12 | 0 << 11  ;        //    enable CONT , set right alignment, set JSWSTART as external trigger and injected conversion on external trigger enable for ADC2

   ADC1->CR1 = 1 << 8 | 5 << 16 ;    //       enable dual mode injected simultaneous mode only, enable scan mode
   ADC2->CR1 = 1 << 8 | 5 << 16  ;    //       enable dual mode injected simultaneous mode only, enable scan mode
   ADC1->SMPR2 = 2 << 3 | 2 << 18 ;   //    set sample time = 13.5 cycle for ADC1 ch1 ch6 --> (Voutsense and Ilinkdc)
   ADC2->SMPR2 = 2 | 2 << 15 | 2 << 24  ;   //    set sample time = 13.5 cycle for ADC2 ch0 ch5 ch8  --->  (temp, Vlinkdc, frequency)
   ADC1->JSQR = 1 << 20 | 1 << 10 | 6 << 15 ;   //   set the length of conversion = 2 ,  set sequence  ch1, ch6
   ADC2->JSQR = 2 << 20 | 8 << 5 | 5 << 10 | 0 << 15 ;   //   set the length of conversion = 3 ,  set sequence  ch8, ch5 , ch0
   ADC1->CR2 |= 1 << 21 ;       //    ADC1 software trigger
    ADC2->CR2 |= 1 << 21;         //    ADC2 software trigger

}


void PWM(void)
{

       	     TIM1->PSC = 0 ;                // prescale = 1 between APB2 and TIMER1 internal clock
			 TIM1->ARR = 0x67B ;            // triangle wave peak value = Timer ARR =1659
		     TIM1->CR1 =  (0 << 9) | (2 << 5) | (0 << 3)  ;     //   set tDTS = tCK_INT , Auto-reload preload disable, Center-aligned mode 2 to have triangle wave (for more information read reference manual), disable one-pulse mode , UEV event generation enable
		     TIM1->CR2 = (0 << 11) | (2 << 4) ;   //  	 set OC1=OC1N=OC2=OC2N=0 after a dead-time when MOE=0	, select update mode in MMS (update event is selected as trigger output (TRGO))
             TIM1->SMCR = 0 << 2 ;  //    disable slave mode
		     TIM1->CCMR1 = (0 << 3) | (6 << 4) | (0 << 7) | (0 << 11) | (6 << 12) | (0 << 15) ;   // set CH1 and CH2 as an output capture, disable preload and fast for Output compare 1 and 2, set PWM mode 1 for CH1 and CH2, SET OC1Ref and OC2Ref is not affected by the ETRF Input
		     TIM1->DIER = 1    ;    //    TIMER1 UPDATE INTERRUPT ENABLE
		     TIM1->CCER = 5 | (5 << 4)    ;    //    enable CH1 and CH1N , enable CH2 and CH2N
		     TIM1->BDTR = 6 | (1 << 7) | (1 << 14)   ;       //  set deadtime = 2us , Automatic outputenable
			 TIM1->CR1 |= 1  ;      //   enable the PWM Timer1
             TIM1->RCR = 1  ;      // to create update event just on the every underflow during each period time
}

void TIM1Interrupt(void)
{

	NVIC->ISER[0]= 1 << 25 ;       // enable update event interrupt of timer 1
    NVIC->IP[25]= 7 ;                  // set 7th priority for update event interrupt of timer 1
}

void UART(void)
{
	USART3->CR1 = 1 << 13    ;                          //   enable USART3
    USART3->BRR = 108 << 4 | 0 << 3  ;               //    19.2Kbps baud rate
	USART3->CR1 |= 1 << 2    ;                     // enable receiver
	USART3->CR1 |= 1 << 3    ;                     //   enable transmitter

}
