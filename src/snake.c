#include <stdio.h>
#include "pico/stdlib.h"
#include "lib/st7735.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define Y_PIN 26
#define X_PIN 27

struct Vec {
    int16_t x;
    int16_t y;
};

const int16_t height = 15;
const int16_t width = 6;
const uint16_t tile_h = (ST7735_HEIGHT-1) / height;
const uint16_t tile_w = (ST7735_WIDTH-1) / width;

int fruitx;
int fruity;
bool fruit = false;

int counter = 1;

void getFruit() {
    fruitx = rand() % width;
    fruity = rand() % height;

    fruit = true;
}

void gameOver(int16_t *x, int16_t *y) {
    *x = width / 2;
    *y = height / 2;

    fruit = false;

    ST7735_FillScreen(ST7735_RED);
    sleep_ms(500);
}

int main() {
    stdio_init_all();  // Initialise serial in/output
    setvbuf ( stdout , NULL , _IONBF , 0 );  // Disable line and block buffering on stdout (for serial comm.)
    sleep_ms(1000);

    adc_init();

    adc_gpio_init(Y_PIN);
    adc_gpio_init(X_PIN);

    ST7735_Init();

    struct Vec direction;
    direction.x = 1;
    direction.y = 0;
    struct Vec pos;
    pos.x = width / 2;
    pos.y = height / 2;

    while (true) {
        // Read y
        adc_select_input(0);
        uint16_t dy = adc_read();

        // Read x
        adc_select_input(1);
        uint16_t dx = adc_read();

        if (dy > 2000) {
            direction.y = 1;
            direction.x = 0;
        } else if (dy < 500) {
            direction.y = -1;
            direction.x = 0;
        } else if (dx > 2500) {
            direction.x = 1;
            direction.y = 0;
        } else if (dx < 500) {
            direction.x = -1;
            direction.y = 0;
        }

        pos.x += direction.x;
        pos.y += direction.y;

        // Handle out of bounds
        if (pos.x == width || pos.x == -1 || pos.y == height || pos.y == -1) {
            gameOver(&pos.x, &pos.y);
        }

        // Handle fruit collection
        if (fruit && fruitx == pos.x && fruity == pos.y) {
            fruit = false;
            counter ++;
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

        // Draw fruit
        if (!fruit) {
            getFruit();
        } else {
            ST7735_FillRectangle((fruitx * tile_w) + 1, (fruity * tile_h) + 1, tile_w - 1, tile_h - 1, ST7735_RED);
        }

        // Draw snake
        ST7735_FillRectangle((pos.x * tile_w) + 1, (pos.y * tile_h) + 1, tile_w - 1, tile_h - 1, ST7735_GREEN);


        sleep_ms(500);
    }
}
