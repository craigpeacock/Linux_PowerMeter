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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <byteswap.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>

uint32_t i2c_init(char *devname)
{
	int handle;

	if ((handle = open(devname, O_RDWR)) < 0) {
		printf("Failed to open I2C port\r\n");
		exit(1);
	}
}

uint32_t i2c_write_byte(uint32_t i2c_master_port, uint8_t address, uint8_t command, uint8_t data)
{

}

uint32_t i2c_write_short(uint32_t i2c_master_port, uint8_t address, uint8_t command, uint16_t data)
{
	uint8_t buffer[3];

	buffer[0] = command;
	buffer[1] = (data & 0xFF00) >> 8;
	buffer[2] = data & 0xFF;

	struct i2c_msg msgs[2];
	struct i2c_rdwr_ioctl_data msgset[1];

	// Message Set 0: Write Command
	msgs[0].addr = address;
	msgs[0].flags = 0;
	msgs[0].len = 3;
	msgs[0].buf = buffer;

	// Message Set contains 1 messages
	msgset[0].msgs = msgs;
	msgset[0].nmsgs = 1;

	if (ioctl(i2c_master_port, I2C_RDWR, &msgset) < 0) {
		printf("Write I2C failed\r\n");
		exit(1);
	}
}

uint32_t i2c_write_buf(uint32_t i2c_master_port, uint8_t address, uint8_t command, uint8_t *data, uint8_t len)
{

}

uint8_t i2c_read_byte(uint32_t i2c_master_port, uint8_t address, uint8_t command)
{

}

uint16_t i2c_read_short(uint32_t i2c_master_port, uint8_t address, uint8_t command)
{
	uint16_t buffer;

	struct i2c_msg msgs[2];
	struct i2c_rdwr_ioctl_data msgset[1];

	// Message Set 0: Write Command
	msgs[0].addr = address;
	msgs[0].flags = 0;
	msgs[0].len = 1;
	msgs[0].buf = &command;

	// Message Set 1: Read 2 bytes
	msgs[1].addr = address;
	msgs[1].flags = I2C_M_RD | I2C_M_NOSTART;
	msgs[1].len = 2;
	msgs[1].buf = (unsigned char *)&buffer;

	// Message Set contains 2 messages
	msgset[0].msgs = msgs;
	msgset[0].nmsgs = 2;

	if (ioctl(i2c_master_port, I2C_RDWR, &msgset) < 0) {
		printf("Read I2C failed\r\n");
		exit(1);
	}

	return(bswap_16(buffer));
}

uint32_t i2c_read_buf(uint32_t i2c_master_port, uint8_t address, uint8_t command, uint8_t *buffer, uint8_t len)
{
	//uint16_t buffer;

	struct i2c_msg msgs[2];
	struct i2c_rdwr_ioctl_data msgset[1];

	// Message Set 0: Write Command
	msgs[0].addr = address;
	msgs[0].flags = 0;
	msgs[0].len = 1;
	msgs[0].buf = &command;

	// Message Set 1: Read len bytes
	msgs[1].addr = address;
	msgs[1].flags = I2C_M_RD | I2C_M_NOSTART;
	msgs[1].len = len;
	msgs[1].buf = buffer;

	// Message Set contains 2 messages
	msgset[0].msgs = msgs;
	msgset[0].nmsgs = 2;

	if (ioctl(i2c_master_port, I2C_RDWR, &msgset) < 0) {
		printf("Read I2C failed\r\n");
		exit(1);
	}

}

