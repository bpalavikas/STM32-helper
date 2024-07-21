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
#ifndef CTI_UDP_SERVER_H_
#define CTI_UDP_SERVER_H_
/******************************************************************************/
/*                                                                            */
/******************************************************************************/

/*--INCLUDES------------------------------------------------------------------*/
#include "lwip/udp.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "cti_debug_logger.h"

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
void CTI_udp_server_init(void);

/**
  * @brief Receive callback function
  * @note This function is called when a UDP packet is received
  * @param arg void pointer 
  * @param pcb pointer to the UDP control block
  * @param p_buffer pointer to the UDP buffer
  * @param addr pointer to the IP address
  * @param port port number
  * @retval None void
  */
void CTI_udp_receive_callback(  void *arg, 
                                struct udp_pcb *pcb,
                                struct pbuf *p_buffer,
                                const ip_addr_t *addr,
                                u16_t port);

/**
  * @brief
  * @note
  * @param
  * @retval
  */




/**
  * @brief
  * @note
  * @param
  * @retval
  */



/**
  * @brief
  * @note
  * @param
  * @retval
  */


/**
  * @brief
  * @note
  * @param
  * @retval
  */

#endif /* */
/******************************************************************************/
/*                             END OF FILE                                    */
/******************************************************************************/
