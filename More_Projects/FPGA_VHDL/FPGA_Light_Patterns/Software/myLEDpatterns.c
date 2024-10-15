#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>
  
#define FATAL do { fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
  __LINE__, __FILE__, errno, strerror(errno)); exit(1); } while(0)
 
#define MAP_SIZE sysconf(_SC_PAGE_SIZE)
#define MAP_MASK (MAP_SIZE - 1)

static volatile int keep_running = 1;

void inthandle(int dummy){
    keep_running = 0;
}

unsigned int pfunk(unsigned long writeval){
    int fd;
    void *map_base, *virt_addr; 
    unsigned long read_result;
    off_t target;    
    
    target = 0xFF20000C;

    if((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) FATAL;
    fflush(stdout);

    /* Map one page */
    map_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, target & ~MAP_MASK);
    if(map_base == (void *) -1) FATAL;

    virt_addr = map_base + (target & MAP_MASK);
    
    *((unsigned short *) virt_addr) = writeval; //going to have to use this for p
    read_result = *((unsigned short *) virt_addr);

    fflush(stdout);
    
    if(munmap(map_base, MAP_SIZE) == -1) FATAL;
    close(fd);
    return read_result;
	
}
char* htb(unsigned long hexval){
	char binaryString[9];
	int i;
	for (i = 0; i < 8; i++){
		binaryString[7-i] = (hexval & (1UL << i)) ? '1' : '0';
	}
	binaryString[i] = '\0';
	return strdup(binaryString);
}

int main(int argc, char **argv) {
    int hflag = 0;
    int vflag = 0;
    int pflag = 0;
    int fflag = 0;
    int opt;
    unsigned long read;
	unsigned char low8;

    if(argc < 2){
	    fprintf(stderr,"Type '-h' for help\n");
	    exit(1);
    }
    while ((opt = getopt(argc, argv, "hvpf")) !=-1){
	    switch (opt) {
		    case 'h':
                hflag = 1;
			    break;
		    case 'v':
                vflag = 1;
			    break;
            case 'p':
                pflag = 1;
                break;
            case 'f':
                fflag = 1;
                break;
		    default:
			    fprintf(stderr, "Illegal data tyle '%c'.\n", opt);
			    exit(2);
		    }
    }
    printf("hflag = %d, vflag = %d, pflag = %d, fflag = %d\n", hflag, vflag, pflag, fflag);
    if (hflag == 1){
        fprintf(stderr,"\nFormat:\t%s { command } [ data ]\n"
			"\tCommands :\n"
			"\t-h    : %s -h: Open the Help Menu\n"
			"\t-p    : %s -p (LED Pattern type Hex Value(i.e 0xFF)) (Time Value in miliseconds (i.e 1000 s)): Displays LED Value in Binary for Amount of time\n"
            "\t-f    : %s -f (file.txt) : Reads in text file of LED patterns and times\n"
            "\t-p    : %s -v (-p or -f): Verbose(-v) can be used to print the values of the leds to the command line for both -p and -f\n\n"
            "\t**NOTE : -p and -f can not be used together\n", argv[0],argv[0],argv[0],argv[0],argv[0]);
    }

    // P Flag Variables
    char *array1[argc/2]; //gets times
    char *array2[argc/2]; //gets patterns
    unsigned long vread;
    char* binval;
    int ar1_idx = 0;
    int ar2_idx = 0;
    if (pflag == 1 && fflag == 0){
        if((argc - optind) % 2 != 0){
            printf(stderr, "ERROR: invalid arguement. Type '-h' for help\n");
            exit(2);
        } 
        for(; optind < argc -1 ; optind += 2){
            array2[ar2_idx] = argv[optind];
            array1[ar1_idx] = argv[optind + 1];
            ar1_idx++;
            ar2_idx++;
        }
        while(keep_running = 1){
            for (int i = 0; i < ar2_idx; i++){
                vread = pfunk(strtoul(array2[i],0,0));
                if(vflag == 1){
                    binval = htb(vread);
                    printf("LED pattern = %s Display time = %d msec\n", binval, atoi(array1[i]));
                    free(binval);
                }
                usleep(atoi(array1[i])*1000);
            }
        }
    }
    
    if (fflag == 1 && pflag == 0){
        int row = 0;
        unsigned long fvread;
        char* fbinval;
        char column1[100][100];
        char column2[100][100];
        FILE *file = fopen(argv[optind], "r");
        if(file == NULL) {
            printf("Not able to open the file.");
            return 1;
        }
        while(fscanf(file, "%s %s", column1[row], column2[row]) == 2){
            row++;
            if(row>= 100){
                break;
            }
        }
        fclose(file);
        for (int i = 0; i < row; i++){
            //printf("Col1 = %s, Col2 = %s\n", column1[i], column2[i]);
            fvread = pfunk(strtoul(column1[i],0,0));
            if(vflag == 1){
                fbinval = htb(fvread);
                printf("LED pattern = %s Display time = %d msec\n", fbinval, atoi(column2[i]));
                free(fbinval);
            }
            usleep(atoi(column2[i])*1000);
        }
    }
    if (pflag == 1 && fflag == 1){
         fprintf(stderr,"ERROR: invalid arguement. Type '-h' for help\n");
        exit(3);
    }
    return (0);
}
