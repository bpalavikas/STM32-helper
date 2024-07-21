/**
********************************************************************************
* @author       Byron Palavikas
* @date 
* @file    
* @brief
* @version      V0.1.0
* @copyright    (C) COPYRIGHT
* @target       STM32
* @IDE
* @repo
*
********************************************************************************
* @attention
*       Custom error codes
********************************************************************************
*/
#ifndef BVR_ERROR_H_
#define BVR_ERROR_H_
/******************************************************************************/
/*                                                                            */
/******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/*--INCLUDES------------------------------------------------------------------*/

/*--DEFINES-------------------------------------------------------------------*/

/*--DATA--TYPE----------------------------------------------------------------*/
/**@enum bool_enum
 * @brief Boolean type definition use as true and false for projects
 * @details Boolean type definition
 */
enum 
{
    BVR_FALSE = 0,      /**< FALSE always set to 0 */
    BVR_TRUE = !BVR_FALSE   /**< TRUE any value that is not 0*/
};


/**@enum BVR_status_t
 * @brief  status type definition
 * @details BVR status type definition
 */
typedef enum
{
    BVR_OK      = 0x00,
    BVR_ERROR   = 0x01,
    BVR_TIMEOUT = 0x02,
    BVR_BUSY    = 0x03,
}BVR_status_t;


/*--GLOBAL--CONSTANTS---------------------------------------------------------*/

/*--MACROS--------------------------------------------------------------------*/

/*--FUNCTION--PROTOTYPE-------------------------------------------------------*/

/**
  * @brief
  * @note
  * @param
  * @retval
  */


#ifdef __cplusplus
}
#endif


#endif /* */
/******************************************************************************/
/*                             END OF FILE                                    */
/******************************************************************************/
