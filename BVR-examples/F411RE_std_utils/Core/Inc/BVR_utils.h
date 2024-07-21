/**
********************************************************************************
*
* @author       Byron Palavikas
* @date
* @file         cti_utils.h
* @brief        standard utilities for most to all projects
* @version      V0.1.0
* @copyright    (C) COPYRIGHT 2023
* @target       STM32 MCU
* @IDE          VSCODE
* @repo
*
*
********************************************************************************
* @attention
*           reset cause should be double checked to suit your MCU 
*           check the reset cause flags in the MCU data sheet
*           and change the enum to suit
*           The UID base address is to be set up for the MCU being used
*           check the data sheet for the base address of the UID
*           and change the define to suit
*           The is rtos active flag is used to set the delay function
            Before the kernal is started the HAL delay is used
            After the kernal is started the rtos delay is used
            Set the is_rtos_active flag in the main.c file right before starting the kernal

            In main 
            Make sure to create the variables for firmware version and reset cause
            and rtos active flag

            Use get resetcause function to directly after the HAL system clock config
            and before the MX init fuctions of peripherals
            In begin user cade 2 after peripherals are init
            get the reset cause string name human readable 
            And dont forget to create the const char *reset_cause_str = NULL;
            at begin user code 1
            reset_cause_t should be declared in private variables

    Variables in main.c
    // data types add to private typedef
    reset_cause_t reset_cause;

    // add to private variables
    char firmware_date[FW_SIZE] = {'\0'};
    const char *reset_cause_str = NULL;

    THIS HAS TO BE HERE!!
   USER CODE BEGIN SysInit
    reset_cause = BVR_get_reset_cause();

   USER CODE BEGIN 2 
    // get the human readable reset cause
    reset_cause_str = BVR_reset_cause_get_name(reset_cause); 

    // get device id    
    BVR_get_unique_ID();
    BVR_calculate_crc(U_ID, U_ID_SIZE, &device_UID);
    // get firmware date
    BVR_create_firmware_date(firmware_date);
    // power on message
    BVR_power_on_information(reset_cause_str, firmware_date, U_ID, device_UID);

********************************************************************************
*/
#ifndef BVR_UTILS_H_
#define BVR_UTILS_H_
/******************************************************************************/
/*                                                                            */
/******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/*--INCLUDES------------------------------------------------------------------*/
// change to suit MCU
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdlib.h> 
#include "BVR_debug_logger.h"


/*--DEFINES-------------------------------------------------------------------*/
#define FW_SIZE     24
#define U_ID_SIZE   12
// Defined by MCU get from data sheet
/* 
BASE ADDRESS FOR STM32 MCU
0x1FFFF7AC //(F302), 0x1FF800D0 //(L152), 0x1FFF7590 //(L433RC), 
0x1FFFF7E8 //(F103), 0x1FF0F420 //(F767), 0x1FFF7A10 // (F411RE)
*/
#define U_ID_BASE_ADDR 0x1FFF7A10 // (F411RE)
  
#define V_MAJOR     0
#define V_MINOR     1
#define V_PATCH     0

#define MONTHS_SIZE 12


/*--DATA--TYPE----------------------------------------------------------------*/

/** Reset cause enum 
   check MCU data sheet change to suit  */
typedef enum
{
    RESET_CAUSE_UNKNOWN = 0,
    RESET_CAUSE_LOW_POWER_RESET,
    RESET_CAUSE_WINDOW_WATCHDOG_RESET,
    RESET_CAUSE_INDEPENDENT_WATCHDOG_RESET,
    RESET_CAUSE_SOFTWARE_RESET,
	RESET_CAUSE_POR_PDR_RESET,
    RESET_CAUSE_EXTERNAL_RESET_PIN_RESET,
    RESET_CAUSE_BROWNOUT_RESET
} reset_cause_t;


/*--GLOBAL--CONSTANTS---------------------------------------------------------*/

// device ID
/** @var uint8_t U_ID[U_ID_SIZE]
 *  @brief 12 byte array for device unique ID
 *  @note  set in cti_utils.c file
 */
extern uint8_t U_ID[U_ID_SIZE];		

/** @var uint32_t device_UID
 *  @brief 32 bit device unique ID
 *  @note  set in cti_utils.c file
 */
extern uint32_t device_UID;			


/*--MACROS--------------------------------------------------------------------*/


/*--FUNCTION--PROTOTYPE-------------------------------------------------------*/


/**
  * @brief Function to get cause of reset from flags set after reset.
  * @note  Change depends on MCU change the enum for MCU used.
  *        Go to definition of RCC_FLAGS to check 
  *        flags in CSR register match function
  *        STM32 defined HAL usually found in stm32f#xx_hal_rcc.h 
  * @param void
  * @retval reset_cause_t enum defined in cti_utils.h
  */
reset_cause_t BVR_get_reset_cause(void);


/**
  * @brief Function returns ASCII readable string of the reset issue
  * @note  MAKE SURE!! Enum defined is correct with MCU used
  * Strings must match reset issue check before use..
  * @param reset_cause_t 
  * @retval char *reset_cause_string
  */
const char * BVR_reset_cause_get_name(reset_cause_t reset_cause);


/**
  * @brief get checksum 
  * @note
  * @param uint8_t *p_data  
  * @param uint32_t length 
  * @param uint32_t *checksum 
  * @retval void 
  */
void BVR_calculate_crc(uint8_t *p_data, uint32_t length, uint32_t *checksum);


/**
  * @brief Get unique ID from MCU unique id register 96bits 
  * @note  Sets the U_ID array which is defined in cti_utils.h with 
  *        12 hex values. 
  *        !IMPORTANT! base address must be set for the MCU being used
  *        Set the defined base address in cti_utils.h file  
  * @param void
  * @retval void
  */
void BVR_get_unique_ID(void);


/**
  * @brief Create a firmware version for application 
  *         saves date of compilation time 
  * @note  
  * @param char *firmware_version  
  * @retval void 
  */
void BVR_create_firmware_date(char *firmware_version);


/**
  * @brief Prints startup information to debug logger 
  * @note  Uses STARTUP log level always on 
  * @param const char *reset_cause_str
  * @param char *firmware_date
  * @param uint8_t *U_ID
  * @param uint32_t device_UID
  * @retval void
  */
void BVR_power_on_information(  const char *reset_cause_str, char *firmware_date,
                                uint8_t *U_ID, uint32_t device_UID);


#ifdef __cplusplus
}
#endif


#endif /* */
/******************************************************************************/
/*                             END OF FILE                                    */
/******************************************************************************/
