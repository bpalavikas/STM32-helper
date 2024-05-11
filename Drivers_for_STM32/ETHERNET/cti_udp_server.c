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


/******************************************************************************/
/*                                                                            */
/******************************************************************************/


/*--INCLUDES------------------------------------------------------------------*/
#include "cti_udp_server.h"
#include "cti_common_defs.h"

/*--DATA--TYPE----------------------------------------------------------------*/

/*--GLOBAL--CONSTANTS---------------------------------------------------------*/

/*--STATIC--DATA--------------------------------------------------------------*/

/*--FUNCTION------------------------------------------------------------------*/


void CTI_udp_server_init(void)
{
    LOG(TRACE, "UDP server init\r\n");

    // set up the control block
    struct udp_pcb *pcb;
    err_t err;

    // create control block
    pcb = udp_new();

    //bind pcb to the local port
    ip_addr_t ipaddr;
    IP4_ADDR(&ipaddr, IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);

    err = udp_bind(pcb, &ipaddr, SOURCE_PORT);

    // set up the receive callback function
    if(err == ERR_OK)
    {
        udp_recv(pcb, CTI_udp_receive_callback, NULL);
    }
    else
    {
        udp_remove(pcb);
    }
}



void CTI_udp_receive_callback(  void *arg, 
                                struct udp_pcb *pcb,
                                struct pbuf *p_buffer,
                                const ip_addr_t *addr,
                                u16_t port)
{
    LOG(TRACE, "UDP server receive callback\r\n");

    // set buffer struct
    struct pbuf *p_tx_buffer;
    // allocate buffer
    char buffer[UDP_CLIENT_BUFFER_SIZE];
    // getting ip from client
    //char *remote_ip = ip4addr_ntoa(addr);

    int len = sprintf(buffer, "Hello from the server! %s\r\n", 
                        (char*)p_buffer->payload);

    // allocate pbuf from the ram 
    p_tx_buffer = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);
    
    // copy data to the pbuf
    pbuf_take(p_tx_buffer, buffer, len);

    // connect to the remote client 
    udp_connect(pcb, addr, port);

    // send reply to the client 
    udp_send(pcb, p_tx_buffer);

    // free the udp connection
    udp_disconnect(pcb);

    // free the pbuf
    pbuf_free(p_tx_buffer);
}





/******************************************************************************/
/*                             END OF FILE                                    */
/******************************************************************************/
