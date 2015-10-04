

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>


#define NB_LEDS 300   
#define PARTICLE_LENGTH 4
#define RED 0
#define GREEN 1
#define BLUE 2

typedef struct pixel_s{
	
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}pixel_t;


const unsigned char particle_kernel[PARTICLE_LENGTH] = {200, 150, 50, 0};

/**
 * main(int argc, char **argv)
 * @brief test the mindbx_lib
 */
int main(int argc, char **argv){
	

	/*define buffer*/
	pixel_t buffer[NB_LEDS];
	int i;
	int spi_driver;
	unsigned char particle_counter = 0x00;
	unsigned char particle_color = 0x00;
	static uint32_t speed = 1000000;
	
	spi_driver = open("/dev/spidev0.0",O_RDWR);
	ioctl(spi_driver, SPI_IOC_WR_MAX_SPEED_HZ, &speed);	
	
	memset(buffer,0,sizeof(pixel_t)*NB_LEDS);
	
	while(1){
		
		/*from the end of red*/
		/*roll back by bringing encountered values forward*/
		for(i=NB_LEDS-1;i>=0;i--){
			buffer[i+1].red = buffer[i].red;
			buffer[i+1].green = buffer[i].green;
			buffer[i+1].blue = buffer[i].blue;
		}
		
		/*check if a particle is being placed*/
		if(particle_counter>0){
			
			switch(particle_color){
				
				case RED:
					buffer[0].red = particle_kernel[particle_counter];
					buffer[0].green = 0;
					buffer[0].blue = 0;
					break;
				case GREEN:
					buffer[0].red = 0;
					buffer[0].green = particle_kernel[particle_counter];
					buffer[0].blue = 0;
					break;
				case BLUE:
					buffer[0].red = 0;
					buffer[0].green = 0;
					buffer[0].blue = particle_kernel[particle_counter];
					break;
			
			}
			particle_counter--;
		}else{
	
			buffer[0].red = 0;
			buffer[0].green = 0;
			buffer[0].blue = 0;
			
			/*else roll a dice to determine if a new particule needs to be spawned*/
			if(((float)rand()/(float)RAND_MAX)>0.66){
				particle_counter = PARTICLE_LENGTH;
				particle_color = rand()%3;
				
				printf("New particle up!\n");
			}
		}	
		
		write(spi_driver, buffer, NB_LEDS*sizeof(pixel_t));
		
		usleep(250000);	
	}
	
	
	exit(0);
}


#if 0
typedef struct pixel_s{
	
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} /*__attribute__((packed)) */pixel_t;


/**
 * main(int argc, char **argv)
 * @brief test the mindbx_lib
 */
int main(int argc, char **argv){
	
	int i, loop_count,start_count;
	int red, blue, green;
	int spi_driver;
	while(1){
	printf("Input the value of start count: ");
	scanf("%d", &start_count);
	printf("Input the value of end count: ");
	scanf("%d", &loop_count);
	printf("Input the Red intensity: ");
	scanf("%d", &red);
	printf("Input the Blue intensity: ");
	scanf("%d", &blue);
	printf("Input the Green intensity: ");
	scanf("%d", &green);
	
	/*define buffer*/
	pixel_t buffer[loop_count];
	
	/*set the whole array to 0*/
	memset(buffer,0,loop_count*sizeof(pixel_t));
	
	for(i=start_count;i<loop_count;i++){
		buffer[i].red = red;
		buffer[i].blue = blue;
		buffer[i].green = green;
	}
	
	
	/*configure the mind box*/
	spi_driver = open("/dev/spidev0.0",O_RDWR);
	write(spi_driver, buffer, loop_count*sizeof(pixel_t));
	
	close(spi_driver);	
	
}
	exit(0);
}
#endif
