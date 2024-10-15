#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>

int16_t fixed_point(int num){ //Shifts by 2
	if (num > 4096){
		num = 4096;
	}else if (num < 0){
		num = 0;
	}
	return (int16_t)(num << 3);
}

int main (int argc, char **argv) {
    int opt;
	// Test reading the regsiters sequentially
	printf("\n***************\n* read initial register values\n***************\n\n");
	
	FILE *file_bass = fopen("/sys/devices/platform/ff207900.bandpassEQ_0/gain_1", "rb+");
    FILE *file_lowmid = fopen("/sys/devices/platform/ff207900.bandpassEQ_0/gain_2", "rb+");
    FILE *file_highmid = fopen("/sys/devices/platform/ff207900.bandpassEQ_0/gain_3", "rb+");
    FILE *file_presence = fopen("/sys/devices/platform/ff207900.bandpassEQ_0/gain_4", "rb+");
	FILE *file_brilliance = fopen("/sys/devices/platform/ff207900.bandpassEQ_0/gain_5", "rb+");
    FILE *file_passthrough = fopen("/sys/devices/platform/ff207900.bandpassEQ_0/passthrough", "rb+");
    FILE *file_volume = fopen("/sys/devices/platform/ff207900.bandpassEQ_0/volume", "rb+");

    int hflag = 0;
    int bflag = 0;
    int lflag = 0;
    int mflag = 0;
    int pflag = 0;
    int iflag = 0;
    int sflag = 0;
    int vflag = 0;
    int rflag = 0;


     if(argc < 2){
	    fprintf(stderr,"Type '-h' for help\n");
	    exit(1);
    }
        while ((opt = getopt(argc, argv, "hblmpisvr")) !=-1){
	    switch (opt) {
            case 'h':
                hflag = 1;
			    break;
		    case 'b':
                bflag = 1;
			    break;
		    case 'l':
                lflag = 1;
			    break;
            case 'm':
                mflag = 1;
                break;
            case 'p':
                pflag = 1;
                break;
             case 'i':
                iflag = 1;
                break;
            case 's':
                sflag = 1;
                break;
            case 'v':
                vflag = 1;
                break;
            case 'r':
                rflag = 1;
                break;
		    default:
			    fprintf(stderr, "Illegal data tyle '%c'.\n", opt);
			    exit(2);
		    }
    
    }

    unsigned int value;

    unsigned int reset = 16384;
    if (argc >2){
        value = atoi(argv[2]);
    }



    if(hflag == 1){ //read from adc
        printf("\nFormat:\t { command } [ data ]\n"
            "\tCommands :\n"
            "\t-h, Help menu.   Usage: '-h'\n"
            "\t-b, Bass.        Usage: '-b <value: 0-65535>'\n"
            "\t-l, Low Mids.    Usage: '-l <value: 0-65535>'\n"
            "\t-m, High Mids.   Usage: '-m <value: 0-65535>'\n"
            "\t-p, Presence.    Usage: '-p <value: 0-65535>'\n"
            "\t-i, Brilliance.  Usage: '-i <value: 0-65535>'\n"
            "\t-s, Passthrough. Usage: '-s <value: 0-65535>'\n"
            "\t-v, Volume.      Usage: '-v <value: 0-65535>'\n"
            "\t-r, Reset.       Usage: '-r'\n");

    }

    if(bflag == 1){ //read from adc
        printf("Bass = %x\n", value);
        fprintf(file_bass, "%d", value);//send value to bass
    }
    if(lflag == 1){ //read from adc
        printf("LowMids = %x\n", value);
        fprintf(file_lowmid, "%d", value);//send value to low mids
    }
    if(mflag == 1){//read from adc
        printf("HighMids = %x\n", value);
        fprintf(file_highmid, "%d", value); //send value to highmids
    }
    if(pflag ==1){//read from adc
        printf("Presence = %x\n", value);
        fprintf(file_presence, "%d", value); //send value to presence
    }
    if(iflag ==1){//read from adc

        printf("Brilliance = %x\n", value); //
        fprintf(file_brilliance, "%d", value); //send value to brilliance
    }
    if(sflag ==1){//read from adc
        printf("Passthrough = %x\n", value); //
        fprintf(file_passthrough, "%d", value); //enable or disable EQ
    }
    if(vflag ==1){//read from adc
        printf("Volume = %x\n", value); //
        fprintf(file_volume, "%d", value); //control volume
    }
    if(rflag ==1){//read from adc
        fprintf(file_passthrough, "%d", 0); //send value to enable
        fprintf(file_volume, "%d", 1); //send value to volume
        fprintf(file_bass, "%d", reset); //send value to bass
        fprintf(file_lowmid, "%d", reset); //send value to lowmids
        fprintf(file_highmid, "%d", reset); //send value to highmids
        fprintf(file_presence, "%d", reset); //send value to presence
        fprintf(file_brilliance, "%d", reset); //send value to brilliance
        printf("Reset All Frequency Bands, Volume and Enable\n");

    }

    fseek(file_bass, 0, SEEK_SET);
    fseek(file_lowmid, 0, SEEK_SET);
    fseek(file_highmid, 0, SEEK_SET);
    fseek(file_presence, 0, SEEK_SET);
    fseek(file_brilliance, 0, SEEK_SET);
    fseek(file_passthrough, 0, SEEK_SET);
    fseek(file_volume, 0, SEEK_SET);
	//close files
    fclose(file_bass);
    fclose(file_lowmid);
    fclose(file_highmid);
    fclose(file_presence);
    fclose(file_brilliance);
    fclose(file_passthrough);
    fclose(file_volume);

	return 0;
}
