/*******************************************************************************
* File Name: controller.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_controller_ALIASES_H) /* Pins controller_ALIASES_H */
#define CY_PINS_controller_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define controller_0			(controller__0__PC)
#define controller_0_PS		(controller__0__PS)
#define controller_0_PC		(controller__0__PC)
#define controller_0_DR		(controller__0__DR)
#define controller_0_SHIFT	(controller__0__SHIFT)
#define controller_0_INTR	((uint16)((uint16)0x0003u << (controller__0__SHIFT*2u)))

#define controller_INTR_ALL	 ((uint16)(controller_0_INTR))


#endif /* End Pins controller_ALIASES_H */


/* [] END OF FILE */
