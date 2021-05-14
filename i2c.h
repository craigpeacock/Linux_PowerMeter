
uint32_t i2c_init(char *devname);

uint8_t i2c_read_byte(uint32_t i2c_master_port, uint8_t address, uint8_t command);
uint32_t i2c_write_byte(uint32_t i2c_master_port, uint8_t address, uint8_t command, uint8_t data);

uint16_t i2c_read_short(uint32_t i2c_master_port, uint8_t address, uint8_t command);
uint32_t i2c_write_short(uint32_t i2c_master_port, uint8_t address, uint8_t command, uint16_t data);

uint32_t i2c_read_buf(uint32_t i2c_master_port, uint8_t address, uint8_t command, uint8_t *buffer, uint8_t len);
uint32_t i2c_write_buf(uint32_t i2c_master_port, uint8_t address, uint8_t command, uint8_t *data, uint8_t len);
