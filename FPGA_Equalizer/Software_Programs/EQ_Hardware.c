//Access ADC registers (pots) and maps them to volume control registers to control the audio effect through hardware
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
	return (int16_t)(num << 3);
}

int main (int argc, char **argv) {
	// Test reading the regsiters sequentially
	printf("\n***************\n* read initial register values\n***************\n\n");

	FILE *file_adc_ch0 = fopen("/sys/devices/platform/ff200000.adc_0/ch0", "r");
	FILE *file_adc_ch1 = fopen("/sys/devices/platform/ff200000.adc_0/ch1", "r");
	FILE *file_adc_ch2 = fopen("/sys/devices/platform/ff200000.adc_0/ch2", "r");
	FILE *file_adc_ch3 = fopen("/sys/devices/platform/ff200000.adc_0/ch3", "r");
    FILE *file_adc_ch4 = fopen("/sys/devices/platform/ff200000.adc_0/ch4", "r");

	
	FILE *file_bass = fopen("/sys/devices/platform/ff207900.bandpassEQ_0/gain_1", "rb+");
    FILE *file_lowmid = fopen("/sys/devices/platform/ff207900.bandpassEQ_0/gain_2", "rb+");
    FILE *file_highmid = fopen("/sys/devices/platform/ff207900.bandpassEQ_0/gain_3", "rb+");
    FILE *file_presence = fopen("/sys/devices/platform/ff207900.bandpassEQ_0/gain_4", "rb+");
	FILE *file_brilliance = fopen("/sys/devices/platform/ff207900.bandpassEQ_0/gain_5", "rb+");


	int ch0;
	int ch1;
	int ch2;
    int ch3;
    int ch4;
    int ch5;

    int16_t fp0;
	int16_t fp1;
	int16_t fp2;
	int16_t fp3;
    int16_t fp4;

	char buffer0[6];
	char buffer1[6];
	char buffer2[6];
    char buffer3[6];
    char buffer4[6];
	
	while(1){
		if(fread(buffer0,2,3, file_adc_ch0)){ //read from adc
			ch0 = atoi(buffer0);
			fp0 = fixed_point(ch0);
			printf("CH0 = %x\n", fp0);
			fprintf(file_brilliance, "%d", fp0);//send value 		}
		if(fread(buffer1,2,3, file_adc_ch1)){ //read from adc
			ch1 = atoi(buffer1);
			fp1 = fixed_point(ch1); //convert by shifting by 2
			printf("CH1 = %x\n", fp1);
			fprintf(file_presence, "%d", fp1);//send value

		}
		if(fread(buffer2,2,3, file_adc_ch2)){//read from adc
			ch2 = atoi(buffer2);
			fp2 = fixed_point(ch2);
			printf("CH2 = %x\n", fp2); //
			fprintf(file_highmid, "%d", fp2); //send value 
		}
        if(fread(buffer3,2,3, file_adc_ch3)){//read from adc
			ch3 = atoi(buffer3);
			fp3 = fixed_point(ch3);
			printf("CH3 = %x\n", fp3); //
			fprintf(file_lowmid, "%d", fp3); //send value 
		}
        if(fread(buffer4,2,3, file_adc_ch4)){//read from adc
			ch4 = atoi(buffer4);
			fp4 = fixed_point(ch4);
			printf("CH4 = %x\n", fp4); //
			fprintf(file_bass, "%d", fp4); //send value to
		}
		else{ //reset addresses

            fseek(file_adc_ch0, 0, SEEK_SET);
            fseek(file_adc_ch1, 0, SEEK_SET);
            fseek(file_adc_ch2, 0, SEEK_SET);
			fseek(file_adc_ch3, 0, SEEK_SET);
			fseek(file_adc_ch4, 0, SEEK_SET);

			fseek(file_bass, 0, SEEK_SET);
			fseek(file_lowmid, 0, SEEK_SET);
			fseek(file_highmid, 0, SEEK_SET);
            fseek(file_presence, 0, SEEK_SET);
            fseek(file_brilliance, 0, SEEK_SET);

		}
	}
	//close files
    fclose(file_adc_ch0);
    fclose(file_adc_ch1);
    fclose(file_adc_ch2);
    fclose(file_adc_ch3);
    fclose(file_adc_ch4);

    fclose(file_bass);
    fclose(file_lowmid);
    fclose(file_highmid);
    fclose(file_presence);
    fclose(file_brilliance);
	return 0;
}
