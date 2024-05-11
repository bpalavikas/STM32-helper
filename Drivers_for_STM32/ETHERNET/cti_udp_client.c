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
#include "cti_udp_client.h"
#include "cti_common_defs.h"



/*--DATA--TYPE----------------------------------------------------------------*/
// control block
struct udp_pcb *client_pcb;


/*--GLOBAL--CONSTANTS---------------------------------------------------------*/
// client_buffer
char client_buffer[UDP_CLIENT_BUFFER_SIZE];
// global count variable
uint32_t g_client_count = 0;



/*--STATIC--DATA--------------------------------------------------------------*/

/*--FUNCTION------------------------------------------------------------------*/

void CTI_udp_client_init(void)
{
    LOG(TRACE, "UDP client init");
    // connect to the server
    // set variables
    ip_addr_t source_ipaddr;
    ip_addr_t netmask_ipaddr;
    ip_addr_t gateway_ipaddr;
    ip_addr_t dest_ipaddr;

    // setup the control block
    client_pcb = udp_new();

    // bind block to port and IP address for source
    IP4_ADDR(&source_ipaddr, IP_ADDR0,IP_ADDR1 ,IP_ADDR2 ,IP_ADDR3);
    IP4_ADDR(&netmask_ipaddr,   NETMASK_ADDR0,
                                NETMASK_ADDR1,
                                NETMASK_ADDR2,
                                NETMASK_ADDR3);
    IP4_ADDR(&gateway_ipaddr,   GATEWAY_ADDR0,
                                GATEWAY_ADDR1,
                                GATEWAY_ADDR2,
                                GATEWAY_ADDR3);
    IP4_ADDR(&dest_ipaddr,  DEST_IP_ADDR0, 
                            DEST_IP_ADDR1, 
                            DEST_IP_ADDR2, 
                            DEST_IP_ADDR3);




}

void CTI_udp_client_connect(void) 
{
    LOG(TRACE, "UDP client connect");

    // variables
    ip_addr_t ipaddr;
    ip_addr_t dest_ipaddr;

    // debug string 
    char *data = "Hello from client";
    
    // error return type
    err_t err;

    // create a new UDP control block
    client_pcb = udp_new();

    // bind block to port and IP address for source
 
    IP4_ADDR(&ipaddr, IP_ADDR0,IP_ADDR1 ,IP_ADDR2 ,IP_ADDR3);
    udp_bind(client_pcb, &ipaddr, SOURCE_PORT);

    // bind block to port and IP address for destination
 
    IP4_ADDR(&dest_ipaddr,  DEST_IP_ADDR0, 
                            DEST_IP_ADDR1, 
                            DEST_IP_ADDR2, 
                            DEST_IP_ADDR3);

    err = udp_connect(client_pcb, &dest_ipaddr, DEST_PORT);

    // set up the receive callback function
    if(err == ERR_OK)
    {
        // send message to server
        CTI_udp_client_send(data);
        // set up the receive callback function
        udp_recv(client_pcb, CTI_udp_client_callback, NULL);
    }
    else
    { 
        LOG(WARN, "UDP client connect failed\tFile: %s\tLine: %d", 
                __FILE__, __LINE__);
        
        udp_remove(client_pcb);
    }
}


void CTI_udp_client_callback(   void *arg, 
                                struct udp_pcb *pcb,
                                struct pbuf *p_buffer,
                                const ip_addr_t *addr,
                                u16_t port)
{
    LOG(TRACE, "UDP client callback");

    // copy data from the buffer
    memcpy(client_buffer, (char *)p_buffer->payload, p_buffer->len);
 
    // print the message
    LOG(TRACE, "Message from server: %s", client_buffer);

    // free the buffer
    pbuf_free(p_buffer);

}


void CTI_udp_client_send(char *data)
{
    LOG(TRACE, "UDP client send");
    // variables 
    int length = 0;

    // set up the buffer
    struct pbuf *p_tx_buffer;
    // allocate buffer
    char buffer[UDP_CLIENT_BUFFER_SIZE];

    // set up the message to formatted string  
    length = sprintf(buffer, "%s %ld", data, g_client_count);

    // allocate pbuf from the ram 
    p_tx_buffer = pbuf_alloc(PBUF_TRANSPORT, length, PBUF_RAM);

    // copy data to the pbuf
    if(p_tx_buffer != NULL)
    {
        pbuf_take(p_tx_buffer, buffer, length);
        // send the message
        udp_send(client_pcb, p_tx_buffer);  
        // free the buffer
        pbuf_free(p_tx_buffer);
    }
    else
    {
        LOG(WARN, "pbuf alloc failed\tFile: %s\tLine: %d", __FILE__, __LINE__);
    }
}



 
/******************************************************************************/
/*                             END OF FILE                                    */
/******************************************************************************/
