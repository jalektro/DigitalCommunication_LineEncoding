#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"


#define CLK_PIN  P9_2
#define DATA_PIN P9_1

#define OUT_POS P9_7
#define OUT_NEG P9_6


volatile bool clock_edge = false;

void clk_isr(void* handler_arg, cyhal_gpio_event_t event)
{
    clock_edge = true;
}

void send_AMI_bit(uint8_t bit)
{
    static bool polarity = false; // false = pos, TRUE = Negative

    if (bit == 0)
    {
        cyhal_gpio_write(OUT_POS, 0);
        cyhal_gpio_write(OUT_NEG, 0);
    }
    else
    {
        if (!polarity)
        {
            cyhal_gpio_write(OUT_POS, 1);
            cyhal_gpio_write(OUT_NEG, 0);
        }
        else
        {
            cyhal_gpio_write(OUT_POS, 0);
            cyhal_gpio_write(OUT_NEG, 1);
        }
        polarity = !polarity;
    }
}

int main(void)
{
    cybsp_init();
	cyhal_gpio_callback_data_t cbData = {.callback = clk_isr };

    cyhal_gpio_init(CLK_PIN, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false);
    cyhal_gpio_init(DATA_PIN, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false);

    cyhal_gpio_init(OUT_POS, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
    cyhal_gpio_init(OUT_NEG, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);

    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);
    cyhal_system_delay_ms(2000);
    printf("=== Bipolaire AMI Receiver gestart ===\r\n");

    cyhal_gpio_register_callback(CLK_PIN, &cbData);
    cyhal_gpio_enable_event(CLK_PIN, CYHAL_GPIO_IRQ_FALL, 3, true);


    uint8_t bit_index = 0;
    uint8_t bitstream[128] = {0};

    __enable_irq();

    for (;;)
    {
        if (clock_edge)
        {
            clock_edge = false;

            bool bit_val = cyhal_gpio_read(DATA_PIN);
            send_AMI_bit(bit_val); // direct AMI uitsturen
/*
            bitstream[bit_index++] = bit_val;

            if (bit_index >= 8)
            {
                uint8_t byte_val = 0;
                for (int i = 0; i < 8; i++)
                    byte_val = (byte_val << 1) | bitstream[i];

                printf("0x%02X\r\n", byte_val);
                bit_index = 0;
            }
*/
        }
    }
}
