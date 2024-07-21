/** 
*******************************************************************************
*
*@author    Byron Palavikas 
*@date      10/09/2023
*@file      debug_logger.c
*@brief     embedded debug logging
*@version   V0.1
*@target    STM32 Devices
*@IDE
*@repo      git@github.com:bpalavikas/STM32-helper.git
*
*
*******************************************************************************
*@attention
*           Refer to header file for more information
*
*******************************************************************************
*/

/******************************************************************************/
/*                                                                            */
/******************************************************************************/

/*--INCLUDES------------------------------------------------------------------*/
#include "BVR_debug_logger.h"
#include "BVR_fifo_buffer.h"

#if SEGGER_DBG
    #include "SEGGER_SYSVIEW.h"
#endif


/*--DATA--TYPE----------------------------------------------------------------*/

log_message_t log_tx_message;
extern fifo_t dbg_uart_tx_fifo;


/*--FUNCTION------------------------------------------------------------------*/

void log_print(const char *fmt, ...)
{
    va_list argp;

    va_start(argp, fmt);
    if(vsprintf((char *)log_tx_message.buffer, fmt, argp) <= 0) return;
    va_end(argp);

    log_tx_message.length = strlen((char *)log_tx_message.buffer);

    #if SEGGER_DBG
    // check for log level
    if(log_tx_message.buffer[0] == 'E')
    {
        SEGGER_SYSVIEW_Error((char *)log_tx_message.buffer);
    }
    else if(log_tx_message.buffer[0] == 'W')
    {
        SEGGER_SYSVIEW_Warn((char *)log_tx_message.buffer);
    }
    else
    {
        SEGGER_SYSVIEW_Print((char *)log_tx_message.buffer);
    }
    #else
    BVR_uart_debug_send((uint8_t*) log_tx_message.buffer, log_tx_message.length);
    #endif
}


void BVR_uart_debug_init(void)
{
    // set buffers for dma
    static uint8_t dbg_uart_rx_buff[UART_BUFFER_LENGTH]; 
    static uint8_t dbg_uart_tx_buff[UART_BUFFER_LENGTH*8];

    // set dma receive buffers
    HAL_UART_Receive_DMA(   &DBG_HUART, 
                            dbg_uart_rx_buff, 
                            ARRAY_SIZE(dbg_uart_rx_buff));

    //disable the interrupts for dma
    __HAL_DMA_DISABLE_IT(&DBG_HDMA_RX, DMA_IT_TC | DMA_IT_HT | DMA_IT_TE);


    //set the tx buffers
    BVR_fifo_init(  &dbg_uart_tx_fifo, 
                    (uint8_t*) dbg_uart_tx_buff,
                    sizeof(dbg_uart_tx_buff));

}

/* To be changed and configured for each project */
void BVR_uart_debug_get(  UART_HandleTypeDef *huart, 
                    char data[],
                    int data_len)
{

    // set 
    volatile size_t head;
    volatile size_t tail;

    char temp_byte; 
    char *p_output_buffer;

    // set the positions for read and write 
    head = huart->RxXferCount;
    tail = (UART_BUFFER_LENGTH) - (huart->hdmarx->Instance->NDTR);

    for(p_output_buffer = data, data_len--; data_len > 0; data_len--)
    {
        if(head != tail)
        { 
            // get byte from uart
            temp_byte = huart->pRxBuffPtr[head++];        

            // head check 
            if(head >= UART_BUFFER_LENGTH){head = 0;}

            // check for end of line 
            if((temp_byte == '\n') | (temp_byte == '\r'))
            {
                // store head position of uart
                huart->RxXferCount = head;
                // null terminate the string 
                p_output_buffer =  '\0'; 
                // get out of here
                return;
            }
            else if(temp_byte == '\b')
            {
                // decrement if backspace 
                p_output_buffer--; 
            }
            else if(temp_byte == '\x1B')
            {
                // erase previous character if special key found 
                p_output_buffer = data;
            }
            else if(isprint(temp_byte))
            {
                *p_output_buffer++ = temp_byte;
            }
        } 
        else 
        { 
            // no data in the buffer
            break;
        }
    }

    // if we are here then data exceeded the max length or is 0
    *p_output_buffer = '\0';
}


void BVR_uart_debug_flush(UART_HandleTypeDef *huart)
{
    // set head and tail for function    
    volatile size_t head = 0; 
    volatile size_t tail = 0;

    // update positions for buffers
    tail = (UART_BUFFER_LENGTH) - (huart->hdmarx->Instance->NDTR);
    head = tail;

    // head check 
    if(head >= UART_BUFFER_LENGTH){head = 0;}

    // store new head postion 
    huart->RxXferCount = head; 
}



BVR_status_t BVR_uart_debug_send(uint8_t *p_data, int size)
{ 
    if(BVR_fifo_push(&dbg_uart_tx_fifo, (uint8_t*) p_data, size) == BVR_OK)
    {
        if(DBG_HUART.gState == HAL_UART_STATE_READY)
        {
            volatile temp_buffer_t dma_temp;
            dma_temp = BVR_fifo_pop_from_temp(&dbg_uart_tx_fifo); 
            HAL_UART_Transmit_DMA(  &DBG_HUART, 
                                    dma_temp.p_temp_buff, 
                                    dma_temp.buff_size);
            // CTI_uart_debug_flush(
            return BVR_OK; 

        }

        return BVR_ERROR; 
    } 

    return BVR_ERROR;
}


/******************************************************************************/
/*                              END OF FILE                                   */
/******************************************************************************/
