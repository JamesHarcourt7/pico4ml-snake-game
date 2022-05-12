#include <stdio.h>
#include "pico/stdlib.h"
#include "lib/st7735.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define Y_PIN 26
#define X_PIN 27


struct Vec {
    uint16_t x;
    uint16_t y;
};

const uint16_t height = 15;
const uint16_t width = 6;
const uint16_t tile_h = (ST7735_HEIGHT-1) / height;
const uint16_t tile_w = (ST7735_WIDTH-1) / width;

int main() {
    stdio_init_all();  // Initialise serial in/output
    setvbuf ( stdout , NULL , _IONBF , 0 );  // Disable line and block buffering on stdout (for serial comm.)
    sleep_ms(1000);

    printf("Hello World!");

    adc_init();

    adc_gpio_init(Y_PIN);
    adc_gpio_init(X_PIN);

    ST7735_Init();

    struct Vec direction;
    direction.x = 1;
    direction.y = 0;
    struct Vec pos;
    pos.x = 0;
    pos.y = 0;

    while (true) {
        printf("aa");
        // Read y
        adc_select_input(0);
        direction.y = adc_read();

        // Read x
        adc_select_input(1);
        direction.x = adc_read();

        char arrayH[5];
        if (direction.y > 2000) {
            pos.y += 1;
        } else if (direction.y < 500) {
            pos.y -= 1;
        } else if (direction.x > 2500) {
            pos.x += 1;
        } else if (direction.x < 500) {
            pos.x -= 1;
        }

        // Do drawing
        ST7735_FillScreen(ST7735_BLACK);

        // Draw each tile
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                ST7735_FillRectangle(i * tile_w, j * tile_h, tile_w, tile_h, ST7735_BLUE);
                ST7735_FillRectangle((i * tile_w) + 1, (j * tile_h) + 1, tile_w - 1, tile_h - 1, ST7735_BLACK);
            }
        }

        // Draw snake
        ST7735_FillRectangle((pos.x * tile_w) + 1, (pos.y * tile_h) + 1, tile_w - 1, tile_h - 1, ST7735_GREEN);

        sleep_ms(500);
    }
}
