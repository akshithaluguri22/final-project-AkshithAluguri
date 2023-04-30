/* Filename   : temperature.c
 * Author     : Akshith Aluguri
 * Description: This file initilizes the i2c device and reads the temperature from the sensor
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define I2C_BUS 1           // I2C bus used on Raspberry Pi
#define TMP102_ADDR 0x48    // I2C address of TMP102 temperature sensor

int file;
char cmdbuf[256];

int init_temp_sensor() {
    char filename[20];
    sprintf(filename, "/dev/i2c-%d", I2C_BUS);

    //Opening the i2c device bus
    if ((file = open(filename, O_RDWR)) < 0) {
        perror("Failed to open the i2c bus");
        return -1;
    }

    if (ioctl(file, I2C_SLAVE, TMP102_ADDR) < 0) {
        perror("Failed to acquire bus access and/or talk to slave");
        return -1;
    }

    return 0;
}

float read_temp() {
    char buf[3];
    float temp;

    // Set pointer register to temperature register
    buf[0] = 0x00;
    if (write(file, buf, 1) != 1) {
        perror("Failed to write to the i2c bus");
        return -1;
    }

    // Read temperature data from register
    if (read(file, buf, 2) != 2) {
        perror("Failed to read from the i2c bus");
        return -1;
    }

    // Convert temperature data to Celsius
    temp = (buf[0] << 4) | (buf[1] >> 4);
    temp *= 0.0625;

    return temp;
}

int main() {
    
    //Initilize the I2c Device
    if (init_temp_sensor() < 0) {
        exit(1);
    }
    float temp=0;
    int status=0;

    while(1){
    	//Read the Temperature into temp
    	temp = read_temp();
    	printf("Temperature: %.2fC\n", temp);

    	//make the command to execute for sending temperature data to server
    	snprintf(cmdbuf, sizeof(cmdbuf), "python3 /bin/mqtt-client.py CurrentTemperature:%.2fC",temp);

    	//Execute the command to send data
    	status = system(cmdbuf);

	usleep(76);

    	if(status){
 		printf("system: error status %d", status);
		goto exit;
    	}     
    }
    	//close the device 
	exit:
	    close(file);
}

