powermeter : main.c i2c.c ina226.c ina228.c
	cc -o powermeter main.c i2c.c ina226.c ina228.c

clean :
	rm powermeter
