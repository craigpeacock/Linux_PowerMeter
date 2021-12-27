/*
 * Linux I2C example code
 * Copyright (C) 2021 Craig Peacock
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <libgen.h>
#include <time.h>
#include "i2c.h"
#include "ina226.h"
#include "ina228.h"

static void print_usage(char *prg)
{
	fprintf(stderr, "Usage: %s [options]\n",prg);
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "	-m <model>		226 or 228\n");
	fprintf(stderr, "	-l <filename> 		Log to file\n");
	fprintf(stderr, "	-p <i2c device> 	I2C port\n");
	fprintf(stderr, "	-a <i2c addr> 		I2C address of power meter (in hex)\n");
	fprintf(stderr, "	-r			Reset device\n");
	fprintf(stderr, "\n");
}

int main(int argc, char **argv)
{
	uint32_t hI2C;
	char * model = "226";
	char * devname = "/dev/i2c-0";
	unsigned char i2caddr = 0x40;
	char * logfilename = NULL;
	bool logtofile = false;
	bool reset = false;

	printf("Power Meter\r\nhttps://github.com/craigpeacock/Linux_I2C\r\n");

	int opt;

	while ((opt = getopt(argc, argv, "m:l:p:a:r?")) != -1) {
		switch (opt) {
			case 'm':
				model = (char *)optarg;
				break;
			case 'l':
				logfilename = (char *)optarg;
				logtofile = true;
				break;
			case 'p':
				devname = (char *)optarg;
				break;
			case 'a':
				i2caddr = (unsigned char) strtol((char *)optarg, NULL, 16);
				break;
			case 'r':
				reset = 1;
				break;

			default:
				print_usage(basename(argv[0]));
				exit(1);
				break;
		}
	}

	FILE *fhandle;

	if (logtofile) {
			printf("Logging to %s\r\n",logfilename);
			fhandle = fopen(logfilename,"a+");
			if (fhandle == NULL) {
				printf("Unable to open %s for writing\r\n",logfilename);
				exit(1);
			}
	}

	printf("\r\nInitialising INA%s at addr 0x%02X on %s \r\n", model, i2caddr, devname);

	hI2C = i2c_init(devname);

	if (reset) {
		printf("Resetting device\r\n");
		i2c_write_short(hI2C, i2caddr, ( strcmp(model,"226") == 0  ? INA226_CFG_REG : INA228_CONFIG) , 0x8000);	// Reset
	}

	//Init
	if ( strcmp(model,"226") == 0 )
	{
		ina226_init(hI2C, i2caddr);
	} else {
		ina228_init(hI2C, i2caddr);
	}

	time_t now;
	struct tm timeinfo;

	do {
		if ( strcmp(model,"226") == 0 )
		{
			printf("Voltage = %.02f V ", ina226_voltage(hI2C, i2caddr));
			printf("Current = %.02f A ", ina226_current(hI2C, i2caddr));
			printf("Power   = %.02f W\r\n", ina226_power(hI2C, i2caddr));
		} else {
			printf("Voltage:         %.03f V\r\n", ina228_voltage(hI2C, i2caddr));
			printf("Die Temperature: %.01f degC\r\n", ina228_dietemp(hI2C, i2caddr));
			printf("Shunt Voltage:   %.03f mV\r\n", ina228_shuntvoltage(hI2C, i2caddr));
			printf("Current:         %.03f A\r\n", ina228_current(hI2C, i2caddr));
			printf("Power:           %.03f W\r\n", ina228_power(hI2C, i2caddr));
			printf("Energy:          %.05f Wh\r\n", ina228_energy(hI2C, i2caddr) / 3600);
			printf("Charge:          %.05f Ah\r\n", ina228_charge(hI2C, i2caddr) / 3600);
		}

		printf("\r\n");

		if (logtofile) {

			time(&now);
			localtime_r(&now, &timeinfo);

			// Timestamp
			fprintf(fhandle,"%04d-%02d-%02d %02d:%02d:%02d,",
					timeinfo.tm_year + 1900,
					timeinfo.tm_mon + 1,
					timeinfo.tm_mday,
					timeinfo.tm_hour,
					timeinfo.tm_min,
					timeinfo.tm_sec);

			if ( strcmp(model,"226") == 0 )
			{
                                fprintf(fhandle, "%.03f,", ina226_voltage(hI2C, i2caddr));
                                fprintf(fhandle, "%.03f,", ina226_current(hI2C, i2caddr));
                                fprintf(fhandle, "%.03f,", ina226_power(hI2C, i2caddr));

			} else {			
				fprintf(fhandle, "%.03f,", ina228_voltage(hI2C, i2caddr));
				fprintf(fhandle, "%.03f,", ina228_current(hI2C, i2caddr));
				fprintf(fhandle, "%.03f,", ina228_power(hI2C, i2caddr));
				fprintf(fhandle, "%.05f,", ina228_energy(hI2C, i2caddr) / 3600);
				fprintf(fhandle, "%.05f", ina228_charge(hI2C, i2caddr) / 3600);
			}
			fprintf(fhandle,"\r\n");
			fflush(fhandle);
		}
		sleep(10);

	} while(1);
}

