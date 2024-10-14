#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

int16_t fixed_point(int num){ //Shifts by 2
	if (num > 4096){
		num = 4096;
	}else if (num < 0){
		num = 0;
	}
	return (int16_t)(num << 2);
}

int main (int argc, char **argv) {
	// Test reading the regsiters sequentially
	printf("\n***************\n* read initial register values\n***************\n\n");
    
	FILE *file_adc_ch2 = fopen("/sys/devices/platform/ff200020.ADC/ch2", "r");
	FILE *file_adc_ch3 = fopen("/sys/devices/platform/ff200020.ADC/ch3", "r");
	FILE *file_adc_ch4 = fopen("/sys/devices/platform/ff200020.ADC/ch4", "r");
	
	FILE *file_red = fopen("/sys/devices/platform/ff200000.HPS_Color_LED/red", "rb+");
    FILE *file_green = fopen("/sys/devices/platform/ff200000.HPS_Color_LED/green", "rb+");
    FILE *file_blue = fopen("/sys/devices/platform/ff200000.HPS_Color_LED/blue", "rb+");

	int ch1;
	int ch2;
	int ch3;

	int16_t fp1;
	int16_t fp2;
	int16_t fp3;

	char buffer1[6];
	char buffer2[6];
	char buffer3[6];
	
	while(1){
		if(fread(buffer1,2,3, file_adc_ch2)){ //read from adc
			ch1 = atoi(buffer1);
			fp1 = fixed_point(ch1);
			printf("CH2 = %x\n", fp1);
			fprintf(file_red, "%d", fp1);//send value to led_red
		}
		if(fread(buffer2,2,3, file_adc_ch3)){ //read from adc
			ch2 = atoi(buffer2);
			fp2 = fixed_point(ch2); //convert by shifting by 2
			printf("CH3 = %x\n", fp2);
			fprintf(file_green, "%d", fp2);//send value to led_green

		}
		if(fread(buffer3,2,3, file_adc_ch4)){//read from adc
			ch3 = atoi(buffer3);
			fp3 = fixed_point(ch3);
			printf("CH4 = %x\n", fp3); //
			fprintf(file_blue, "%d", fp3); //send value to led_blue
		}
		else{ //reset addresses

            fseek(file_adc_ch2, 0, SEEK_SET);
			fseek(file_adc_ch3, 0, SEEK_SET);
			fseek(file_adc_ch4, 0, SEEK_SET);

			fseek(file_red, 0, SEEK_SET);
			fseek(file_green, 0, SEEK_SET);
			fseek(file_blue, 0, SEEK_SET);

		}
	}
	//close files
	fclose(file_adc_ch2);
    fclose(file_adc_ch3);
    fclose(file_adc_ch4);
	fclose(file_red);
	fclose(file_green);
	fclose(file_blue);
	return 0;
}
