/**
  ******************************************************************************
  * @file    usbh_hid_mouse.h
  * @author  MCD Application Team
  * @version V3.2.1
  * @date    26-June-2015
  * @brief   This file contains all the prototypes for the usbh_hid_mouse.c
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_HID_PS3_H
#define __USBH_HID_PS3_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbh_hid.h"

/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_CLASS
  * @{
  */

/** @addtogroup USBH_HID_CLASS
  * @{
  */

/** @defgroup USBH_HID_PS3
  * @brief This file is the Header file for usbh_hid_mouse.c
  * @{
  */


/** @defgroup USBH_HID_PS3_Exported_Types
  * @{
  */

typedef struct _HID_PS3_Info
{
  uint8_t              x;
  uint8_t              y;
  uint8_t              buttons[3];
}
HID_PS3_Info_TypeDef;

/**
  * @}
  */

/** @defgroup USBH_HID_PS3_Exported_Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_HID_PS3_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_HID_PS3_Exported_Variables
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_HID_PS3_Exported_FunctionsPrototype
  * @{
  */
USBH_StatusTypeDef USBH_HID_PS3Init(USBH_HandleTypeDef *phost);
HID_PS3_Info_TypeDef *USBH_HID_GetPS3Info(USBH_HandleTypeDef *phost);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBH_HID_PS3_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
