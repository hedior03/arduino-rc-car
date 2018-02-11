#include <16F1823.h> 
#DEVICE ADC=10 
#FUSES NOWDT, NOWRT, NOLVP, NOCPD, NOPROTECT 
#FUSES MCLR, PUT, INTRC_IO, NOBROWNOUT 
#use delay(clock=16000000)
#use fast_io(C)
#USE RS232(baud=57600, xmit=PIN_C4, rcv=PIN_C5)
//#use rs232(uart1, baud=9600)


void main() 
{
   setup_oscillator(OSC_16MHZ|OSC_PLL_OFF); 
   delay_us(20);
   
   set_tris_c(0x00);
   output_c(0x00);
   
   setup_adc_ports(NO_ANALOGS);
   setup_adc_ports(sAN0|/*sAN1|*/sAN2|sAN3|VSS_VDD);  //TODOS PINES COMO ENTRADAS ANALOGAS
   setup_adc(ADC_CLOCK_INTERNAL);   //CONFIG VELOCIDAD DE CONVERSION
   
   int16 acc = 0,
            dirX = 0,
            dirY = 0,
            yaw = 0;
   
   int1 revF = 0;
               
   int16 cnt = 0;
   
   while(TRUE){
      set_adc_channel(0);  //LECTURA ANALOGA 0
      delay_us(10);  //revisar clock
      acc = read_adc();
      
      set_adc_channel(2);  //LECTURA ANALOGA 2
      delay_us(10);  //revisar clock
      dirX = read_adc();
      
      set_adc_channel(3);  //LECTURA ANALOGA 3
      delay_us(10);  //revisar clock
      yaw = read_adc();
      //===================================================== 
      if (acc<=51){
         if((yaw>512)){
            acc = -((yaw-512)/3);
            revF = 1;
         }else{
            acc = 0;
            revF = 0;
         }
      }else revF = 0;
      //===================================================== 
      printf("$;A=%04ld; B=%04ld;*",acc,dirX);
      delay_ms(150);
      
      //=====================================================
      if ((acc>51)){//LED DE ACC CERCANA A CERO
         output_low(PIN_C1);  //SI ESTA ACEERANDO
      }else if(acc >= 0){
         output_high(PIN_C1);
      }
      //=====================================================
      if ((dirX>485)&&(dirX<537)){//LED DE DIRECCION CENTRADA
         output_high(PIN_C2);
      }else{
         output_low(PIN_C2);
      }
      //=====================================================
      cnt++;
      if (cnt==50){  //THROB
         output_toggle(PIN_C0);
         if (revF){
            output_toggle(PIN_C1);// EN CASO DE REVERSAR.... BLINK
         }
         cnt=0;
      }
   }
}
