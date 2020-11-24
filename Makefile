execute: 
	gcc ./src/main.c ./inc/bme/bme280.c \
		./src/modules/csv_module.c \
		./src/modules/gpio_module.c \
		./src/modules/lcd_module.c \
		./src/modules/uart_module.c \
		./src/modules/t_module_i2c.c \
		./src/modules/user.c \
	 	-I ./inc/bme \
		-I ./inc/lcd -lbcm2835\
		-lpthread \
		-lncurses \
		-lwiringPi \
		-o ./bin/trabalho1
	./bin/trabalho1
