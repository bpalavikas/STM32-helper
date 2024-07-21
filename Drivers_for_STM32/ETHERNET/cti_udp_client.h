/**
********************************************************************************
*                CTI - Centre for Technology Infusion
* @author 
* @date 
* @file    
* @brief
* @version
* @target
* @IDE
* @repo
*
********************************************************************************
* @attention
*
********************************************************************************
*/
#ifndef CTI_UDP_CLIENT_H_
#define CTI_UDP_CLIENT_H_

/******************************************************************************/
/*                                                                            */
/******************************************************************************/

/*--INCLUDES------------------------------------------------------------------*/
#include "lwip/udp.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "cti_debug_logger.h"
#include "stm32f7xx_hal.h"
#include "main.h"

/*--DATA--TYPE----------------------------------------------------------------*/


/*--GLOBAL--CONSTANTS---------------------------------------------------------*/

/*--DEFINES-------------------------------------------------------------------*/

/*--MACROS--------------------------------------------------------------------*/

/*--FUNCTION--PROTOTYPE-------------------------------------------------------*/

/**
  * @brief Set up the UDP server
  * @note This function sets up the UDP server
  * @param None void
  * @retval None void
  */
void CTI_udp_client_connect();

/**
  * @brief Receive callback function
  * @note This function is called when a UDP packet is received
  * @param void *arg void pointer
  * @param struct udp_pcb_t *pcb pointer to the UDP control block
  * @param struct pbuf *p_buffer pointer to the UDP buffer
  * @param const ip_addr_t *addr pointer to the IP address
  * @param u16_t port port number
* @retval None void
*/
void CTI_udp_client_callback(   void *arg, 
                                struct udp_pcb *pcb,
                                struct pbuf *p_buffer,
                                const ip_addr_t *addr,
                                u16_t port);


/**
  * @brief Send a UDP packet
  * @note This function sends a UDP packet
  * @param None void
  * @retval None void
  */
void CTI_udp_client_send(char *data);

/**
  * @brief
  * @note
  * @param
  * @retval
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);




#endif /* */
/******************************************************************************/
/*                             END OF FILE                                    */
/******************************************************************************/
