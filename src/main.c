#include <MKL25Z4.h>
#include <math.h>
#include <stdio.h>
#include "utils.h"
#define x1 240




void PORTA_IRQHandler(void);
void enable_PTA13_interrupt(void) {
    PORTA->PCR[13] |= PORT_PCR_IRQC(0x0A); // Falling edge interrupt
}

void disable_PTA13_interrupt(void) {
    PORTA->PCR[13] &= ~PORT_PCR_IRQC_MASK; // Disable interrupt
}

volatile unsigned int control;
void PORTA_IRQHandler(void) {
// clear pending interrupts
NVIC_ClearPendingIRQ(PORTA_IRQn);
if (PORTA->ISFR) {

    if (PORTA->ISFR & (1 << 1)) {
       enableLED(0);
			control++;
			if (control==2){
			control = 0;
	}

    }

    if (PORTA->ISFR & (1 << 12)) {
			if (control == 0){
				control = 3;
			}
			if (control == 1){
				control = 4;
			}
			
}
	if (PORTA->ISFR & (1 << 13)) {
		if (control == 10) {
			control = 12; 
			disable_PTA13_interrupt();
		}
		PORTA -> ISFR = (1 << 13); }
// clear status flags

PORTA->ISFR = 0xffffffff;
}
}
int main() {
		led_init();
		initGPIO();
		adc_init(12); 
 		LCD_init();
		init_IRQPTA1();
		pwm(1966);
		pwmm(1966);
		control = 2;
		print_fnc("GATE IS EMPTY");
		unsigned int avg_data = 0;
		unsigned int tt_data = 0;
		uint32_t time = 0;
		uint32_t timet = 0;
		uint32_t speed;
 while(1){
		if (control == 2) {
			triggerSensor(7);
      float distance = measureDistance(0);
				if (distance <= 32){
					delay_us(650000);
					control = 0;
		
					
					}
			else if (control == 10) {
				enable_PTA13_interrupt();
			}
			else{
					delay_us(650000);
					pwm(1966);
					pwmm(1966);
					
					}	
			
			
		}
		else if(control==1){
			uint32_t data = adc_read(0);
			avg_data = data;
			char buffer[11]; // Enough to store max uint32_t value + null terminator
			uint32_to_string(data, buffer);
			delay_us(100000);
			LCD_command(0x01);  
			print_fnc("SPEED_LIMIT: ");
			print_fnc(buffer);
		}
		else if(control==0){
			uint32_t data = adc_read(0);
			tt_data = data;
			char buffer[11]; // Enough to store max uint32_t value + null terminator
			uint32_to_string(data, buffer);
			delay_us(100000);
			LCD_command(0x01);  
			print_fnc("TIME_TRIAL: ");
			print_fnc(buffer);
		}
		
		else if(control==3){
				LCD_command(0x01);
				print_fnc("SYSTEM ACTIVATED");
				delay_us(70000);
				triggerSensor(7);
      float distance = measureDistance(0);
				if (distance >= 32){
					delay_us(650000);
					pwm(1966);
					pwmm(1966);
					control = 6;
					
					}
			else{
				pwm(655);
					
					}	
		}
		else if(control==6){
				char buffer[11]; // Enough to store max uint32_t value + null terminator
				uint32_to_string(time, buffer);
				delay_us(50000);
				triggerSensor(3);
      float distance = measureDistance(4);
				if (distance <= 32){
					delay_us(2500);
					control = 7;
					
					}
			else{
					delay_us(650000);
					pwmm(1966);
					}	
				LCD_command(0x01);  
				print_fnc("TIME: ");
				print_fnc(buffer);
				time++;
			
		}
		
		else if(control==7) {
					LCD_command(0x01);
					print_fnc("WANTED TIME: ");
					
					char buffer[11]; // Enough to store max uint32_t value + null terminator
					uint32_to_string(tt_data, buffer);
					print_fnc(buffer);
					LCD_command(0xC0);
					print_fnc("TIME SPENT: ");
					
					uint32_to_string(time, buffer);
				
					print_fnc(buffer);	
					delay_us(650000);
					control = 11;

					
		}
		
		else if(control==4){ // AVARAGE 
				LCD_command(0x01);
				print_fnc("SYSTEM ACTIVATED");
				delay_us(700000);
				triggerSensor(7);
      float distance = measureDistance(0);
				if (distance < 32){
					pwm(655);
					
					}
				else {
					pwm(1965);
					control = 8;
				}
	
	}
		else if(control==8){ // AVARAGE 
			
			char buffer[11]; // Enough to store max uint32_t value + null terminator
			uint32_to_string(timet, buffer);
			LCD_command(0x01);  
			print_fnc("SPEED CALCULATING");
			delay_us(700000);
			print_fnc(buffer);
			timet++;
			
			pwm(1966);
			triggerSensor(3);
      float distance = measureDistance(4);
				if (distance <= 32){
							control = 9;
					}


	}
		
	
	
		else if(control==9) {
					LCD_command(0x01);
					print_fnc("SPD_LIMIT: ");
					char buffer[11]; // Enough to store max uint32_t value + null terminator
					uint32_to_string(avg_data, buffer);
					print_fnc(buffer);
					LCD_command(0xC0);
					print_fnc("SPEED: ");
					speed = x1 / timet;
					uint32_to_string(speed, buffer);
					print_fnc(buffer);	
					print_fnc(" km/h");
					delay_us(6000);
					control = 10;
		}
		else if(control==10) {
					if (speed > avg_data){
						
						LCD_command(0x01);
						print_fnc(" SPEED LIMIT       ");
						LCD_command(0xC0);
						print_fnc("   EXCEEDED!!          ");
						
						LCD_command(0x01);
						print_fnc("   PAYMENT        ");
						LCD_command(0xC0);
						print_fnc("    NEEDED       ");
						
						
					}
					else {
							delay_us(2000);
							pwmm(655);
							control = 11;
					}
					}
		else if (control == 11){

						LCD_command(0x01);
						print_fnc("HAVE A GOOD TRIP         ");
									triggerSensor(3);
      float distance = measureDistance(4);
				if (distance >= 32){
					delay_us(6500);
				
					pwmm(1966);
					}
			else{
					delay_us(6500);
					pwmm(655);
					
					}	
					
						control = 2;
								}
						
						
					
		else if (control == 12) {
			
						print_fnc("   PAYMENT         ");
						LCD_command(0xC0);
						print_fnc("   RECEIVED      ");
						delay_us(1000);
						control = 11;
					
	}

		
		
	
	/*while (1) {
			
        triggerSensor();
        float distance = measureDistance();
		if (distance <= 32){
				delay_us(650000);
			pwm(655);
		}
			else{
				delay_us(650000);
			pwm(1966);
			}	
		}*/
}
}
