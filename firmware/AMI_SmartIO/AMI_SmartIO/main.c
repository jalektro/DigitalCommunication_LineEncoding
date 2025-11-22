

/*******************************************************************************
* Header Files
*******************************************************************************/

#include "cyhal.h"
#include "cybsp.h"
#include "cy_pdl.h"
#include "cy_retarget_io.h"


void handle_error (uint32_t status) {
    /* Board init failed. Stop program execution */
    if (status != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
}

int main(void)
{
    cy_rslt_t result;
    cy_en_smartio_status_t status;

#if defined (CY_DEVICE_SECURE) && defined (CY_USING_HAL)
    cyhal_wdt_t wdt_obj;

    /* Clear watchdog timer so that it doesn't trigger a reset */
    result = cyhal_wdt_init(&wdt_obj, cyhal_wdt_get_max_timeout_ms());
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    cyhal_wdt_free(&wdt_obj);
#endif

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    handle_error(result);

    result= cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,CY_RETARGET_IO_BAUDRATE);
    handle_error(result);
    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
      printf("\x1b[2J\x1b[;H");


      printf("*************** SmartIO: AMI line encoding *************** \r\n\n");
    /* Enable global interrupts */
    __enable_irq();

    status = Cy_SmartIO_Init(smart_io_HW, &smart_io_config);

       if(CY_SMARTIO_SUCCESS != status)
       {
           printf("SmartIO init failed with error: %lu\r\n",(unsigned long) status);
           CY_ASSERT(0);
       }

       /* Enable the Smart I/O */
       Cy_SmartIO_Enable(smart_io_HW);
       printf("*************** SmartIO: start encoding *************** \r\n\n");
    for (;;)
    {
    }
}

