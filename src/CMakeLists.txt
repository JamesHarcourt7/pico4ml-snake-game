add_executable(
	snake
		snake.c
	lib/fonts.c
	lib/st7735.c
	lib/DEV_Config.c
)

# pull in common dependencies
target_link_libraries(
	snake
	hardware_pwm
	hardware_pio
	hardware_spi
	hardware_i2c
	hardware_adc
	pico_stdlib
)

# create map/bin/hex file etc.
pico_add_extra_outputs(snake)

# add url via pico_set_program_url
example_auto_set_url(snake)

