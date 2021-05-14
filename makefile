powermeter : main.c i2c.c ina226.c
	cc -o powermeter main.c i2c.c ina226.c

clean :
	rm powermeter
