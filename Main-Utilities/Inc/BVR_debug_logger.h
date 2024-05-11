/**
*******************************************************************************
*
* @author       Byron Palavikas 
* @date         10/09/2023
* @file         debug_logger.h
* @brief        embedded debug logging
* @version      V1.0.0
* @copyright    (C) COPYRIGHT
* @target       STM32 MCU
* @IDE          NVIM
* @repo
*
*
*******************************************************************************
*@attention
*   This is the debug logger 
    Warning levels are set 
    TRACE to trace the flow of the firmware show function entry and exit

    DBG use for debug prints that are only to make sure something is working 
    getting the values of sensors

    INFO use to log info that is not an error and above debug values like
    start stop services connection to pipes and ports

    WARN use for error checking on potential issues that should have correction 
    in the code  

    ERR errors that are fatal to an operation that can not be specifically 
    corrected in the code, but system operation will continue

    FATAL when the system has crashed and something can not be recovered 
    hardfault errors watchdog forced restarts error handlers

    STARTUP is only used for start up information like firmware version
    and any other information always needed at start up this will always print
    no matter what the log level is set to so do not put sensitive information here
    Use DBG for sensetive start up information 

    To set an ID for each print in the c file add const char* ID = <"Sensor">
    different ID can be set and passed. 

    This uses the BVR_fifo_buffer.h so make sure you have this included in your project
    Make sure to change the defines for the uart and dma handles to match settings and
    the MCU
    If using segger system view make sure to include everything and set the define 
*    
*   EXAMPLE
*   static const char* ID = "CO2";
*   CTI_LOG(INFO, ID, "This is the INFO message");
*   
*   OUTPUT
*   INFO <-> CO2 	This is the INFO message
*
*   EXAMPLE WIHT NO ID
*   CTI_LOG(TRACE, "This is the TRACE message");
*
*   OUTPUT
*   TRACE <-> This is the TRACE message 
*
*   In main.c
*   Make sure to create fifo_t dbg_uart_tx_fifo; in private variables
*   EAXAMPLE CODE FOR MAIN.C

// Private typedef 
fifo_t dbg_uart_tx_fifo;


// USER CODE BEGIN 0
// set up the call back for the uart and fifo
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{ 
    volatile temp_buffer_t temp_dma;
    if(huart == &DBG_HUART)
    {
        // pop from the fifo 
        temp_dma = CTI_fifo_pop_from_temp(&dbg_uart_tx_fifo);
        // Send to the DMA
        HAL_UART_Transmit_DMA(  &DBG_HUART, 
                                temp_dma.p_temp_buff, 
                                temp_dma.buff_size);
    }
}

// USER CODE BEGIN 2
#if !SEGGER_DBG
    // init the uart debug
    CTI_uart_debug_init();
#endif

// OTHER CODE HERE

// SEGGER SYSTEM VIEW
// THIS MUST BE CALLED AFTER EVERYTHING ELSE
// ONLY MX_FREERTOS_Init() then osKernelStart() should be called after this
#if SEGGER_DBG
    SystemInit();
    SEGGER_SYSVIEW_Conf();
    SEGGER_SYSVIEW_Start();
#endif

*   
*
*******************************************************************************
*/
#ifndef BVR_DEBUG_LOGGER_H_
#define BVR_DEBUG_LOGGER_H_
/******************************************************************************/
/*                                                                            */
/******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/*--INCLUDES------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <ctype.h>
#include <stdarg.h>
// Change for MCU
#include "stm32f7xx_hal.h"

/*--DEFINES-------------------------------------------------------------------*/
#define LOG_BUFFER_SIZE 200

// debug levels
#define TRACE   6   /**< print out everything all */
#define DBG     5   /**< for debug */        
#define INFO    4   /**< information like functions where code is*/    
#define WARN    3   /**< warnings */     
#define ERR     2   /**< error this is bad */        
#define FATAL   1   /**< System issue */
#define STARTUP 1   /**< start up information on firmware version */


/*--PLATFORM-CONF-------------------------------------------------------------*/
// Set log level
#define LOG_LEVEL TRACE
// Set segger Logging = 1 UART = 0
#define SEGGER 0

#define ARRAY_SIZE(A) (sizeof(A)/sizeof(A[0]))
#define UART_BUFFER_LENGTH 256
#define STRING_LENGTH 128

/** This is to be set dependent on your MCU and uart*/
#define DBG_HUART   huart3
#define DBG_HDMA_RX hdma_usart3_rx
#define DBG_HDMA_TX hdma_usart3_tx


/*--MACROS--------------------------------------------------------------------*/

#define BVR_LOG(level, id, ...) _LOG_ ## level(id, __VA_ARGS__)

#if LOG_LEVEL >= TRACE 
#define _LOG_TRACE(id, ...) __LOG(TRACE, id, __VA_ARGS__)
#else
#define _LOG_TRACE(...)
#endif

#if LOG_LEVEL >= DBG
#define _LOG_DBG(id, ...) __LOG(DBG, id, __VA_ARGS__)
#else
#define _LOG_DBG(...)
#endif

#if LOG_LEVEL >= INFO
#define _LOG_INFO(id, ...) __LOG(INFO, id, __VA_ARGS__)
#else
#define _LOG_INFO(...)
#endif

#if LOG_LEVEL >= WARN
#define _LOG_WARN(id, ...) __LOG(WARN, id, __VA_ARGS__)
#else
#define _LOG_WARN(...)
#endif

#if LOG_LEVEL >= ERR 
#define _LOG_ERR(id, ...) __LOG(ERR, id, __VA_ARGS__)
#else
#define _LOG_ERR(...)
#endif

#if LOG_LEVEL >= FATAL 
#define _LOG_FATAL(id, ...) __LOG(FATAL, id, __VA_ARGS__)
#else
#define _LOG_FATAL(...)
#endif

#if LOG_LEVEL >= STARTUP
#define _LOG_STARTUP(id, ...) __LOG(STARTUP, id, __VA_ARGS__)
#else
#define _LOG_STARTUP(...)
#endif



#define __LOG(level, id, format, ...)   log_print(#level " <-> %s \t:" format "\r\n",id, ## __VA_ARGS__)


/*--DATA--TYPE----------------------------------------------------------------*/
// external uart dma handles types
extern UART_HandleTypeDef huart3; 
extern DMA_HandleTypeDef  hdma_usart3_rx;
extern DMA_HandleTypeDef  hdma_usart3_tx;

/**@struct log_buffer_t
 * @brief log buffer holds log message to send to uart
 * log buffer */
typedef struct
{
    uint8_t     buffer[LOG_BUFFER_SIZE]; /** buffer for log message */
    int         size; /** size of buffer */
}log_buffer_t;


/** @struct log_message_t
 * @brief log message holds log message to send to uart more detailed
 * log message */
typedef struct
{
    log_buffer_t    log_buffer; /** buffer for log message */ 
    uint8_t         buffer[LOG_BUFFER_SIZE]; /** buffer for log message */
    uint16_t        msg_type; /** type of message */
    int             length; /** length of message */
}log_message_t;


/** @enum  uart_debug_status_t
 * @brief uart debug status for error checking
 * Debug status */
typedef enum 
{
    DBG_SUCCESS,
    DBG_ERROR
}uart_debug_status_t; 


/*--FUNCTION--PROTOTYPE-------------------------------------------------------*/

/**
* @brief Formatted string function for debug log and segger logs
* @note  Define if segger or debug uart in debug_logger.h 
* @param  const char *fmt, ...
* @retval void 
*/
extern void log_print(const char *fmt, ...);


/**
  * @brief Sets the buffers for uart and sets fifo pointers to tx buffers
  *        Will disable the Interrupts for DMA
  * @note !Make sure correct uart handles are set for uart and dma 
  * @param void
  * @retval void
  */
void BVR_uart_debug_init(void);


/**
* @brief Gets characters from DMA rx moving to buffer
*        looking for escape characters terminates on null terminator  
* @note
* @param  UART_HandleTypeDef *huart
* @param char data[]
* @param int data_len 
* @retval void
*/
void BVR_uart_debug_get(  UART_HandleTypeDef *huart, 
                        char data[],
                        int data_len);


/**
* @brief Resets head and tail pointers with huart NDTR reg 
*        resets uart handle rx counter 
* @note
* @param UART_HandleTypeDef *huart
* @retval void
*/
void BVR_uart_debug_flush(UART_HandleTypeDef *huart);


/**
* @brief Pushes fifo buffer then will pop from temp buffer to send to the DMA
* @note ! waits for ready state if uart is busy will miss the sent buffer
* @param uint8_t *p_data 
* @param int size
* @retval uart_debug_status_t
*/
uart_debug_status_t BVR_uart_debug_send(uint8_t *p_data, int size);


#ifdef __cplusplus
}
#endif

 
#endif /*debug_logger.h*/
/******************************************************************************/
/*                              END OF FILE                                   */
/******************************************************************************/
