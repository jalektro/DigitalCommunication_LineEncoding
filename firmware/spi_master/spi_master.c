#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19

int main()
{
    stdio_init_all();
    uint8_t test = 0xAA;
    uint8_t read;

    // SPI initialisation. This example will use SPI at 10KHz.
    spi_init(SPI_PORT, 4000000);

    // Configure SPI format: 8 data bits, CPOL=0 (clock idle low), CPHA=0 (sample on rising edge)
    spi_set_format(SPI_PORT, 8, SPI_CPOL_0, SPI_CPHA_1, SPI_MSB_FIRST);

    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS,   GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    
    // Chip select is active-low, so we'll initialise it to a driven-high state
    // For more examples of SPI use see https://github.com/raspberrypi/pico-examples/tree/master/spi

    while (true) {
        spi_write_read_blocking(SPI_PORT,&test,&read,1);
    }
}