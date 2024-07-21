/**
********************************************************************************
* @author       Aidin Bervan 
* @date 
* @file         cti_fifo_buffer.h
* @brief        CTI fifo buffer for the library 
* @version      V0.1.0
* @copyright    (C) COPYRIGHT
* @target       ARM STM32
* @IDE          NEOVIM
* @repo
*
*
********************************************************************************
* @attention
*           In main make sure to add the callback for the uart 
*           void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
*           {
*               volatile temp_buffer_t temp_dma;
*               if(huart == &DBG_HUART)
*               {
*                   temp_dma = BVR_fifo_pop_from_temp(&dbg_uart_tx_fifo);
*                   HAL_UART_Transmit_DMA(&DBG_HUART, temp_dma.p_temp_buff, temp_dma.buff_size);
*               }
*           }
*
*           void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
*           {
*               UNUSED(huart);
*           }
*
*           You will also need to add the fifo_t dbg_uart_tx_fifo; to the private variables
*           When you are using the uart debug logger 
*
********************************************************************************
*/
#ifndef BVR_FIFO_BUFFER_H_
#define BVR_FIFO_BUFFER_H_
/******************************************************************************/
/*                                                                            */
/******************************************************************************/
#ifdef __cplusplus
    extern "C" {
#endif

/*--INCLUDES------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "BVR_error.h"


/*--DATA--TYPE----------------------------------------------------------------*/

/**@struct fifo_control_t
 * @brief fifo control type definition
 * @details fifo control type definition
 */        
typedef struct
{
    int depth;  /**< fifo depth */
    int level;  /**< fifo level */
    int head;   /**< fifo head */
    int tail;   /**< fifo tail */
}fifo_control_t;

/**@struct fifo_t
 * @brief fifo type definition
 * @details fifo type definition
 */
typedef struct
{
    fifo_control_t ctrl;    /**< fifo control */
    uint8_t *p_buffer;      /**< fifo buffer pointer */
}fifo_t;

/**@struct temp_buffer_t
 * @brief temp buffer type definition
 * @details temp buffer type definition
 */
typedef struct
{
    uint8_t *p_temp_buff;  /**< temp buffer pointer */
    int     buff_size;     /**< temp buffer size */
}temp_buffer_t;



/*--FUNCTION--PROTOTYPE-------------------------------------------------------*/

/**
  * @brief Initialise fifo struct 
  * @note
  * @param fifo_t *fifo
  * @param uint8_t buffer[] 
  * @param int depth
  * @retval fifo_status_t
  */
extern BVR_status_t BVR_fifo_init(fifo_t *fifo, uint8_t buffer[], int depth);

/**
  * @brief Push data to the fifo buffer 
  * @note
  * @param fifo_t *fifo
  * @param uint8_t *data
  * @param int buffer_size
  * @retval fifo_status_t
  */
extern BVR_status_t BVR_fifo_push(fifo_t *fifo, uint8_t *data, int buffer_size);

/**
  * @brief Pop data from fifo buffer
  * @note
  * @param 
  * @retval fifo_status_t
  */
extern BVR_status_t BVR_fifo_pop(fifo_t *fifo, uint8_t *data, int buffer_size);

/**
  * @brief Pop data from fifo to temp buffer 
  * @note  used for dma transfers
  * @param fifo_t *fifo
  * @retval temp_buffer_t
  */
extern temp_buffer_t BVR_fifo_pop_from_temp(fifo_t *fifo);



#ifdef __cplusplus
}
#endif   

#endif /* */
/******************************************************************************/
/*                             END OF FILE                                    */
/******************************************************************************/
