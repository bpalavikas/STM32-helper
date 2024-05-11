# SEGGER SYSTEM VIEW RTT INSTRUCTIONS

Systemview is a tool that allows you to see the RTOS in action. It allows you to see the tasks and the
time they take to execute. It also allows you to see the interrupts and the time they take to execute.

For all print statements use the LOG() function provided by the CTI libraries. 
This will allow you to print to the debug uart or the segger debug.

The segger warning, error, and print will be automatically sent to the segger debug.
The error and warn levels will correspond to the segger debug levels.
Every other level will be sent as print. 

## DEPENDENCIES
### Segger debug tools must be used
You can flash a nucleo board to a segger jlink follow link for downloads and instructions

https://www.segger.com/products/debug-probes/j-link/models/other-j-links/st-link-on-board/

Or use segger jlink *instructions coming soon for this*

### Use CTI UTILS FILES
Download the zip file. Extract and copy CTI_ c files and header files
These files contain everything needed for debug uart and segger debug
Use the LOG() print function for all prints in your project once the SEGGER_DBG
DEFINE is set to 1 all LOG() will be sent to the segger. 

Set up the CTI files as instructed in headers.

### SEGGER FOLDER STRUCTURE
Place the SEGGER folder into
* Middlewares
    * Third_Party
        * SEGGER

## SETUP INCLUDE PATHS
### CUBE IDE
After adding files to the project you may need to right-click the project then do a refresh 
to make sure all new files and folders are seen in the project. 

Next right click the project select properties 

In the popup screen navigate to C/C++ Build then go to Settings

In tool settings Make sure the include paths have the new segger headers in the include path 
This will need to be done for both MCU GCC Assembler and MCU GCC Compiler

Both of the these will have the folder include paths in their drop-down menus. Select the include path
then in the right select the green cross in the popup window go to workspace and navigate to the 
include folders in the segger folder. You can only select one at a time, so it will have to be done 
for each include folder. One in SEGGER then other in the Target folder. 

### VSCODE
You may have to restart or build the project you have to make sure cmake has the folders for middlewares
included. Cmake will do everything for you. 
Cmake can miss some things if not added to cmake file automatically add below manually. 

    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:ASM>>:${PROJECT_SOURCE_DIR}/Middlewares/Third_Party/SEGGER/Include>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:ASM>>:${PROJECT_SOURCE_DIR}/Middlewares/Third_Party/SEGGER/Target/Include>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Middlewares/Third_Party/SEGGER/Target/Include>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Middlewares/Third_Party/SEGGER/Include>"

## FREERTOS CONFIG
The freertos config file will be in the Inc folder under Core (presuming you have built from MXCube)
In the file FreeRTOSConfig.h you need to add the following. 
```
/* USER CODE BEGIN Includes */
/* Section where include file can be added */
 #include "SEGGER_SYSVIEW_FreeRTOS.h"
```
Then at the bottom of the file add 

```
/* USER CODE BEGIN Defines */
/* Section where parameter definitions can be added (for instance, to override default ones in FreeRTOS.h) */
#define INCLUDE_xTaskGetIdleTaskHandle 1
#define INCLUDE_pxTaskGetStackStart 1
```

Make sure to keep them in these user sections if cubeMX rebuilds they will be removed. 


## SEEGER_SYSVIEW.C names
Go to this file and then and in the defines, configurable section there will two defines
```
#define SYSVIEW_APP_NAME        add project name here

#define SYSVIEW_DEVICE_NAME     add the board name here
```
## ==!IMPORTANT!== 
In the stm32f1xx_hal_msp.c file which is found in Src

Add the following line of code. 
```
  /* USER CODE BEGIN MspInit 1 */
  NVIC_SetPriorityGrouping( 0 );
  /* USER CODE END MspInit 1 */
```

# EXAMPLE CODE 

Make sure #define SEGGER_DBG is set to 1 In debug_logger.h


inside main.c

```
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "cti_common_defs.h"
#include "cti_error_code.h"
#include "cti_fifo_buffer.h"
#include "cti_debug_logger.h"
#include "cti_uart_handler.h"
#include "cti_utils.h"

#if SEGGER_DBG
	#include "SEGGER_SYSVIEW.h"
	#include "SEGGER_SYSVIEW_Conf.h"
#endif


/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
fifo_t dbg_uart_tx_fifo;
reset_cause_t reset_cause;


/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
char *firmware_date = NULL;


/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	volatile temp_buffer_t temp_dma;
	if(huart == &DBG_HUART)
	{
		temp_dma = CTI_fifo_pop_from_temp(&dbg_uart_tx_fifo);
		HAL_UART_Transmit_DMA(&DBG_HUART, temp_dma.p_temp_buff, temp_dma.buff_size);
	}
}

void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	UNUSED(huart);
}


  /* USER CODE BEGIN 1 */
	const char *reset_cause_str = NULL;
  /* USER CODE END 1 */


  /* USER CODE BEGIN Init */
reset_cause = CTI_get_reset_cause();
  /* USER CODE END Init */


  /* USER CODE BEGIN 2 */
#if !SEGGER_DBG
  // get the human readable reset cause
  reset_cause_str = CTI_reset_cause_get_name(reset_cause);
  // init the debug logger
  CTI_uart_debug_init();

  // get device id
  CTI_get_unique_ID();
  CTI_calculate_crc(U_ID, U_ID_SIZE, &device_UID);
  // get firmware date
  CTI_create_firmware_date(firmware_date);
  // power on message
  CTI_power_on_information(reset_cause_str, firmware_date, U_ID, device_UID);
#endif

  // SEGGER
#if SEGGER_DBG
  SystemInit();
  SEGGER_SYSVIEW_Conf();
  SEGGER_SYSVIEW_Start();
#endif

  /* USER CODE END 2 */
```





