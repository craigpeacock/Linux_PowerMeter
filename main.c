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
#include "i2c.h"
#include "ina226.h"

int main(int argc, char **argv)
{
	uint32_t hI2C;

	hI2C = i2c_init("/dev/i2c-1");
	ina226_init(hI2C);

	do {

		printf("Voltage = %.02f V ", ina226_voltage(hI2C));
		printf("Current = %.02f A ", ina226_current(hI2C));
		printf("Power   = %.02f W\r\n", ina226_power(hI2C));
		sleep(1);

	} while(1);
}

