/**
********************************************************************************
* @author   Aidin Bervan 
* @date     
* @file    cti_fifo_buffer.c
* @brief   FIFO buffer implementation
* @version V0.1
* @target  STM32
* @IDE
* @repo    git@github.com:bpalavikas/STM32-helper.git
*
********************************************************************************
* @attention
*       Refer to header file for more information
********************************************************************************
*/


/******************************************************************************/
/*                                                                            */
/******************************************************************************/


/*--INCLUDES------------------------------------------------------------------*/

#include "BVR_fifo_buffer.h" 


/*--DATA--TYPE----------------------------------------------------------------*/

/*--GLOBAL--CONSTANTS---------------------------------------------------------*/

/*--STATIC--DATA--------------------------------------------------------------*/

/*--FUNCTION------------------------------------------------------------------*/


BVR_status_t BVR_fifo_init(fifo_t *fifo, uint8_t buffer[], int depth)
{
    if(buffer != NULL)
    {
        fifo->p_buffer = buffer;
        fifo->ctrl.depth = depth;
        fifo->ctrl.level = 0x00;
        fifo->ctrl.head  = 0x00;
        fifo->ctrl.tail  = 0x00;
        return BVR_OK;
    }
    else
    {
        // TODO repalce with non dma 
        // HAL_UART_Transmit(&DBG_HUART, 
        return BVR_ERROR; 
    }
}


BVR_status_t BVR_fifo_push(fifo_t *fifo, uint8_t *data, int buffer_size)
{
    // Set function variables 
    int end_of_buffer   = 0;
    int count           = 0;
    int head            = fifo->ctrl.head;
    // set empty buffer level
    end_of_buffer = fifo->ctrl.depth - fifo->ctrl.level;

    // loop through buffer
    if(end_of_buffer >= buffer_size)
    {
        for(count = 0; count < buffer_size; count++)
        {
            fifo->p_buffer[head++] = data[count];

            // reset head
            if(head >= fifo->ctrl.depth){head = 0;}
        }

        fifo->ctrl.head = head;
        fifo->ctrl.level += buffer_size;
        return BVR_OK;
    }
    else
    {
        // TODO repalce with non dma 
        // HAL_UART_Transmit(&DBG_HUART, 
        return BVR_ERROR;
    }

}


BVR_status_t BVR_fifo_pop(fifo_t *fifo, uint8_t *data, int buffer_size)
{ 
    // set variables 
    int count   = 0;
    int tail    = fifo->ctrl.tail;
    int level   = fifo->ctrl.level;
    int depth   = fifo->ctrl.depth;


    if(level >= buffer_size)
    {
        for(count = 0; count < buffer_size; count++)
        {
            data[count] = fifo->p_buffer[tail++];

            if(tail>= depth){ tail = 0;}
        }

        fifo->ctrl.tail = tail;
        fifo->ctrl.level -= buffer_size; 
        return BVR_OK;
    } 
    else
    {
        // TODO repalce with non dma 
        // HAL_UART_Transmit(&DBG_HUART, 
        return BVR_ERROR; 
    }
}


temp_buffer_t BVR_fifo_pop_from_temp(fifo_t *fifo)
{
    // set temp variables to volatile
    volatile int depth = fifo->ctrl.depth;
    volatile int level = fifo->ctrl.level;
    volatile int tail  = fifo->ctrl.tail;
    volatile int buffer_size;
    volatile temp_buffer_t ret_buffer;

    if(level > 0)
    {
        if((tail + level) < depth)
        {
            buffer_size = level; 
        }
        else
        {
            buffer_size = depth - tail; 
        }

        ret_buffer.p_temp_buff = fifo->p_buffer + tail;
        ret_buffer.buff_size   = buffer_size; 
        level -= buffer_size; 
        tail  += buffer_size;

        if(tail >= depth){ tail = 0;}

        fifo->ctrl.level = level; 
        fifo->ctrl.tail  = tail;
    }
    else
    {
        ret_buffer.p_temp_buff = NULL; 
        ret_buffer.buff_size = 0; 
    }

    return ret_buffer; 
}



/******************************************************************************/
/*                                UART                                        */
/******************************************************************************/
/* 
 * EXAMPLES For setting up UART 
 *
 *


void BVR_fifo_uart_init(UART_HandleTypeDef huart, 
                        DMA_HandelTypeDef dma_rx, 
                        fifo_t fifo_tx)
{
    // set buffers for dma
    static uint8_t uart_rx_buff[UART_BUFFER_LENGTH]; 
    static uint8_t uart_tx_buff[UART_BUFFER_LENGTH*8];

    // set dma receive buffers
    HAL_UART_Receive_DMA(   &huart, 
                            uart_rx_buff, 
                            ARRAY_SIZE(uart_rx_buff));

    //disable the interrupts for dma
    __HAL_DMA_DISABLE_IT(&dam_rx, DMA_IT_TC | DMA_IT_HT | DMA_IT_TE);


    //set the tx buffers
    BVR_fifo_init(  &fifo_tx, 
                    (uint8_t*) uart_tx_buff,
                    sizeof(uart_tx_buff));

}


void BVR_fifo_uart_get(  UART_HandleTypeDef *huart, 
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


void BVR_fifo_uart_flush(UART_HandleTypeDef *huart)
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



BVR_status_t BVR_fifo_uart_send(fifo_t fifo_tx, 
                                UART_HandleTypeDef huart, 
                                uint8_t *p_data, 
                                int size)
{ 
    if(BVR_fifo_push(&fifo_tx, (uint8_t*) p_data, size) == FIFO_SUCCESS)
    {
        if(huart.gState == HAL_UART_STATE_READY)
        {
            volatile temp_buffer_t dma_temp;
            dma_temp = BVR_fifo_pop_from_temp(&uart_tx_fifo); 
            HAL_UART_Transmit_DMA(  &huart, 
                                    dma_temp.p_temp_buff, 
                                    dma_temp.buff_size);
            return BVR_OK; 
        }

        return BVR_ERROR; 
    } 

    return BVR_ERROR;
}

*/

/******************************************************************************/
/*                             END OF FILE                                    */
/******************************************************************************/
