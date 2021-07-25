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
#include <libgen.h>
#include "i2c.h"
#include "ina226.h"
#include "ina228.h"

static void print_usage(char *prg)
{
	fprintf(stderr, "Usage: %s [options]\n",prg);
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "	-p <i2c device> 	I2C port\n");
	fprintf(stderr, "	-a <i2c addr> 		I2C address of power meter (in hex)\n");
	fprintf(stderr, "\n");
}

int main(int argc, char **argv)
{
	uint32_t hI2C;
	char * devname = "/dev/i2c-0";
	unsigned char i2caddr = 0x40;

	printf("Power Meter\r\nhttps://github.com/craigpeacock/Linux_I2C\r\n");

	int opt;

	while ((opt = getopt(argc, argv, "p:a:?")) != -1) {
		switch (opt) {
			case 'p':
				devname = (char *)optarg;
				break;
			case 'a':
				i2caddr = (unsigned char) strtol((char *)optarg, NULL, 16);
				break;

			default:
				print_usage(basename(argv[0]));
				exit(1);
				break;
		}
	}

	printf("\r\nInitialising device at addr 0x%02X on %s \r\n", i2caddr, devname);

	hI2C = i2c_init(devname);

	//ina226_init(hI2C, i2caddr);
	ina228_init(hI2C, i2caddr);

	do {

		//printf("Voltage = %.02f V ", ina226_voltage(hI2C, i2caddr));
		//printf("Current = %.02f A ", ina226_current(hI2C, i2caddr));
		//printf("Power   = %.02f W\r\n", ina226_power(hI2C, i2caddr));

		printf("Voltage:         %.03f V\r\n", ina228_voltage(hI2C, i2caddr));
		printf("Die Temperature: %.01f degC\r\n", ina228_dietemp(hI2C, i2caddr));
		printf("Shunt Voltage:   %.03f mV\r\n", ina228_shuntvoltage(hI2C, i2caddr));
		printf("Current:         %.03f A\r\n", ina228_current(hI2C, i2caddr));
		printf("Power:           %.03f W\r\n", ina228_power(hI2C, i2caddr));
		printf("Energy:          %.05f Wh\r\n", ina228_energy(hI2C, i2caddr) / 3600);
		printf("Charge:          %.05f Ah\r\n", ina228_charge(hI2C, i2caddr) / 3600);
		printf("\r\n");

		sleep(1);

	} while(1);
}

